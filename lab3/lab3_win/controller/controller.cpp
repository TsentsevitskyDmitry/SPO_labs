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

const char* corePath = "child.exe";


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
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	vector<HANDLE> writeOrder;
	vector<HANDLE> closeOrder;
	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Print");
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Close");

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
				break;

			case '-':
				popWriter(writeOrder, closeOrder);
				break;

			case 'q':
				run = false;
				disposeWriters(writeOrder, closeOrder);
				ReleaseSemaphore(close, 1, NULL);
				CloseHandle(print);
				CloseHandle(close);
				continue;
			}
		}

		if (WaitForSingleObject(print, 10) == WAIT_OBJECT_0)
		{
			if (writeOrder.size() < 1)
				ReleaseSemaphore(print, 1, NULL);
			
			else 
			{
				currentWriter + 1 < writeOrder.size() ? ++currentWriter : currentWriter = 0;
				ReleaseSemaphore(writeOrder[currentWriter], 1, NULL);
			}
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
	writeOrder.push_back(CreateSemaphore(NULL, 0, 1, string("PrintController").append(eventName).c_str()));
	closeOrder.push_back(CreateSemaphore(NULL, 0, 1, string("CloseController").append(eventName).c_str()));
}

void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder)
{
	if (writeOrder.size() < 1)
		return;

	CloseHandle(writeOrder.back());
	writeOrder.pop_back();

	ReleaseSemaphore(closeOrder.back(), 1, NULL);
	CloseHandle(closeOrder.back());
	closeOrder.pop_back();
}

void disposeWriters(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder)
{
	for (int i = 0; i < writeOrder.size(); ++i)
	{
		ReleaseSemaphore(closeOrder[i], 1, NULL);
		CloseHandle(closeOrder[i]);
		CloseHandle(writeOrder[i]);
	}
}