#include "controller.h"

using namespace std;

void pushWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder, string unicString);
void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);
void disposeWriters(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);

ChildArgs args;

DWORD WINAPI controller_main(LPVOID lpParam)
{
	string unicStrings[] = {
		"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
		"In quam nisl, tempus et rutrum id, sagittis et ligula.",
		"Vivamus fringilla enim elementum nisi malesuada, in rutrum justo elementum.",
		"Donec aliquet fringilla lorem, quis venenatis enim porttitor eget.",
		"Aenean id orci non risus hendrerit rhoncus nec ac mi.",
		"Duis in aliquam nisl. Mauris lobortis ante mauris, nec rhoncus erat dapibus ac.",
		"Vivamus nulla justo, finibus id sollicitudin im perdiet, dictum eu elit. Duis quis neque enim.",
		"In ipsum quam, venenatis non tincidunt ut, consectetur in lectus. Donec semper neque at interdum pharetra.",
		"Praesent vel sodales sem, ac pretium magna. Nunc blandit augue lacus, nec mollis nibh eleifend ut.",
		"Quisque mollis lectus lorem, vitae iaculis neque rutrum eu.",
		"Donec quis nisl vitae justo interdum lobortis ac vel nisl.",
		"Pellentesque maximus lectus ultrices, rutrum velit vel, mollis est."
	};
	const int unicCount = sizeof(unicStrings) / sizeof(string);

	vector<HANDLE> writeOrder;
	vector<HANDLE> closeOrder;
	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Print");
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Close");
	HANDLE access = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Access");

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
	}
	return 0;
}

void pushWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder, string unicString)
{
	static int id;
	string syncName = to_string(id++);
	args.syncName = syncName;
	args.unicString = unicString;
	HANDLE thread = CreateThread(NULL, 0, child_main, &args, 0, NULL);

	writeOrder.push_back(thread);
	closeOrder.push_back(CreateSemaphore(NULL, 0, 1, string("CloseController").append(syncName).c_str()));
	Sleep(50);
}

void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder)
{
	if (writeOrder.size() < 1) 
		return;

	ReleaseSemaphore(closeOrder.back(), 1, NULL);
	CloseHandle(closeOrder.back());
	closeOrder.pop_back();

	WaitForSingleObject(writeOrder.back(), INFINITE);
	writeOrder.pop_back();
	Sleep(50);
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