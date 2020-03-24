#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>
#include "../libs/extprocess.h"

using namespace std;

void pushWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder, string unicString);
void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);
void disposeWriters(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);

const char* corePath = "parent.exe";


int main() 
{
	string unicStrings[] = {
		"\"Lorem ipsum dolor sit amet, consectetur adipiscing elit.\"",
		"\"In quam nisl, tempus et rutrum id, sagittis et ligula.\"",
		"\"Vivamus fringilla enim elementum nisi malesuada, in rutrum justo elementum.\"",
		"\"Donec aliquet fringilla lorem, quis venenatis enim porttitor eget.\"",
		"\"Aenean id orci non risus hendrerit rhoncus nec ac mi.\"",
		"\"Duis in aliquam nisl. Mauris lobortis ante mauris, nec rhoncus erat dapibus ac.\"",
		"\"Vivamus nulla justo, finibus id sollicitudin imperdiet, dictum eu elit. Duis quis neque enim.\"",
		"\"In ipsum quam, venenatis non tincidunt ut, consectetur in lectus. Donec semper neque at interdum pharetra.\"",
		"\"Praesent vel sodales sem, ac pretium magna. Nunc blandit augue lacus, nec mollis nibh eleifend ut.\"",
		"\"Quisque mollis lectus lorem, vitae iaculis neque rutrum eu.\"",
		"\"Donec quis nisl vitae justo interdum lobortis ac vel nisl.\"",
		"\"Pellentesque maximus lectus ultrices, rutrum velit vel, mollis est.\"",
		"\"Proin non fermentum ex. Curabitur vitae neque nibh.\"",
		"\"Donec fringilla porta diam tincidunt tempus. Phasellus lobortis hendrerit ligula.\"",
		"\"Curabitur lorem justo, bibendum vel tellus a, venenatis aliquet lorem.\"",
		"\"Donec ultrices scelerisque lacus eget pulvinar.\"",
		"\"Sed elementum diam velit, eu efficitur nisl hendrerit ac.\"",
		"\"Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.\"",
		"\"Phasellus lectus felis, maximus vitae eros ornare, faucibus posuere turpis.\"",
		"\"In hac habitasse platea dictumst. Nullam ut nisi interdum, fringilla est vitae, mattis eros.\"",
		"\"Suspendisse ut imperdiet orci. Aliquam sed libero venenatis, lobortis dolor eget, auctor nibh.\"",
		"\"Sed viverra blandit ligula porttitor scelerisque.\"",
		"\"Morbi blandit, felis vitae lacinia lobortis, dolor arcu feugiat risus, vitae lobortis purus lectus eu lectus.\"",
		"\"Cras convallis tempor fringilla. Maecenas mollis erat euismod dui pretium euismod.\"",
		"\"Donec at vestibulum justo, euismod tincidunt dolor. In luctus non erat ac egestas. Etiam at accumsan erat.\"",
		"\"Donec faucibus aliquam porttitor. Duis vitae sagittis enim. Nullam fermentum nunc ante, ac tempus odio viverra sed.\"",
		"\"Ut sollicitudin, est at rutrum semper, neque arcu vulputate odio, eu porta tellus nisl id sapien.\"",
		"\"Nunc pretium sapien est. Cras eleifend porta lorem, nec luctus eros. Integer in vulputate augue.\"",
		"\"Etiam fringilla tellus varius tristique bibendum. Phasellus nec tempor nisl, ut fermentum nunc.\"",
		"\"Ut pellentesque euismod tincidunt. Proin id magna orci.\"",
		"\"Nam ante felis, iaculis eu sollicitudin sed, dignissim et est.\"",
		"\"Sed semper metus vitae bibendum egestas. Proin commodo ligula nulla, vel dapibus justo faucibus vel.\"",
		"\"Nullam auctor pellentesque ex. Curabitur sed dui cursus, iaculis purus eu, feugiat risus.\"",
		"\"Ut sed odio enim. Ut dapibus velit sit amet magna pharetra, a imperdiet elit imperdiet.\"",
		"\"Quisque faucibus lorem eget justo rhoncus sodales. Etiam auctor odio eget tempus ullamcorper.\""
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	vector<HANDLE> writeOrder;
	vector<HANDLE> closeOrder;
	HANDLE synñhronize = CreateEvent(NULL, TRUE, TRUE, "synhronize");
	int currentWriter = 0;

	bool run = true;
	while (run) 
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case '+':
				pushWriter(writeOrder, closeOrder, unicStrings[writeOrder.size() % unicCount]);
				if (writeOrder.size() == 1)
					if (WaitForSingleObject(synñhronize, 1) == WAIT_OBJECT_0)
						SetEvent(writeOrder[0]);
				break;
			case '-':
				popWriter(writeOrder, closeOrder);
				break;
			case 'q':
				disposeWriters(writeOrder, closeOrder);
				run = false;
				break;
			}
		}


		if (WaitForSingleObject(synñhronize, 100) == WAIT_OBJECT_0)
		{
			if (writeOrder.size() < 1)
				continue;

			ResetEvent(synñhronize);
			currentWriter + 1 < writeOrder.size() ? ++currentWriter : currentWriter = 0;
			SetEvent(writeOrder[currentWriter]);
		}		
	}
	return 0;
}

void pushWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder, string unicString)
{
	static int id;
	ExtProcess ext;
	string eventName = to_string(id++);
	ext.createProcess(corePath, eventName + " " + unicString, false);
	writeOrder.push_back(CreateEvent(NULL, TRUE, FALSE, eventName.append("_write").c_str()));
	closeOrder.push_back(CreateEvent(NULL, TRUE, FALSE, eventName.append("_close").c_str()));
}

void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder)
{
	if (writeOrder.size() < 1)
		return;

	CloseHandle(writeOrder.back());
	writeOrder.pop_back();

	SetEvent(closeOrder.back());
	CloseHandle(closeOrder.back());
	closeOrder.pop_back();
}

void disposeWriters(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder)
{
	for (int i = 0; i < writeOrder.size(); ++i)
	{
		CloseHandle(writeOrder[i]);
		CloseHandle(closeOrder[i]);
	}
}