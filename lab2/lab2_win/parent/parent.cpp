#include <iostream>
#include <windows.h>
#include "VendingMachine.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32 | _WIN64
	system("chcp 1251 > nul");
#endif
    
	if (argc < 3) return -1;

	string eventName = string(argv[1]);
	string unicString = string(argv[2]);
	HANDLE write = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName.append("_write").c_str());
	HANDLE close = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName.append("_close").c_str());
	HANDLE synñhronize = OpenEvent(EVENT_ALL_ACCESS, FALSE, "synhronize");

	while (1)
	{
		if (WaitForSingleObject(write, 100) == WAIT_OBJECT_0)
		{
			for (int i = 0; i < unicString.length(); ++i)
			{
				cout << unicString[i];
				Sleep(50);
			}
			cout << endl;

			ResetEvent(write);
			SetEvent(synñhronize);
		}

		if (WaitForSingleObject(close, 100) == WAIT_OBJECT_0)
		{
			CloseHandle(write);
			CloseHandle(close);
			CloseHandle(synñhronize);
			return 0;
		}
	}

	//VendingMachine machine;
	//machine.ConstRequest();

	return 0;
}
