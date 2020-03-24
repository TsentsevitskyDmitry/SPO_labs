#include "child.h"

DWORD WINAPI child_main(LPVOID lpParam)
{
	ChildArgs* args = (ChildArgs*)lpParam;

	string sem = args->syncName;
	char client_string[256];
	strcpy(client_string, args->unicString.c_str());

	//HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, string("PrintController").append(sem).c_str());
	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Print");
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, string("CloseController").append(sem).c_str());
	HANDLE reset = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Reset");
	HANDLE access = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Access");

	HANDLE conn = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		"FileMap");
	LPVOID buff = MapViewOfFile(conn,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);

	while (true)
	{
		if (WaitForSingleObject(access, 100) == WAIT_OBJECT_0)
		{
			if (WaitForSingleObject(print, 10) == WAIT_OBJECT_0)
			{
				strncpy((char*)buff, client_string, 256);
				ReleaseSemaphore(reset, 1, NULL);

				while(WaitForSingleObject(print, 10) != WAIT_OBJECT_0)
					Sleep(10);
				ReleaseSemaphore(print, 1, NULL);

				ReleaseMutex(access);
				Sleep(100);
			}
		}

		if (WaitForSingleObject(close, 10) == WAIT_OBJECT_0)
		{
			CloseHandle(close);
			CloseHandle(print);
			CloseHandle(reset);
			UnmapViewOfFile(buff);
			CloseHandle(conn);
			return 0;
		}

	}
	return 0;
}