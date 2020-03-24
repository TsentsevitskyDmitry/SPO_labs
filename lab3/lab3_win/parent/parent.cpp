#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
//#include "VendingMachine.h"
#include "../libs/extprocess.h"

const char* corePath = "controller.exe";


int main()
{
#ifdef _WIN32 | _WIN64
	system("chcp 1251 > nul");
#endif
	char server_buffer[256];
	HANDLE print = CreateSemaphore(NULL, 0, 1, "Print");
	HANDLE reset = CreateSemaphore(NULL, 0, 1, "Reset");
	HANDLE close = CreateSemaphore(NULL, 0, 1, "Close");

	HANDLE	conn = CreateFileMapping(INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		256,
		"FileMap");
	LPVOID buff = MapViewOfFile(conn,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		256);

	ExtProcess controller;
	controller.createProcess(corePath, "", false);
	ReleaseSemaphore(print, 1, NULL);

	while (1)
	{
		if (WaitForSingleObject(reset, 10) == WAIT_OBJECT_0)
		{
			strncpy(server_buffer, (char*)buff, 256);
			for (int i = 0; i < strlen(server_buffer); ++i)
			{
				cout << server_buffer[i];
				Sleep(50);
			}
			cout << endl;
			ReleaseSemaphore(print, 1, NULL);
		}

		if (WaitForSingleObject(close, 10) == WAIT_OBJECT_0)
		{
			CloseHandle(print);
			CloseHandle(reset);
			CloseHandle(close);
			UnmapViewOfFile(buff);
			CloseHandle(conn);
			controller.wait(INFINITE);
			return 0;
		}
	}

	//VendingMachine machine;
	//machine.ConstRequest();

	return 0;
}
