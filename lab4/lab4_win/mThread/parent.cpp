#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>
#include "controller.h"

using namespace std;

int main() 
{
	system("chcp 1251 > nul");

	char server_buffer[256];

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

	HANDLE print = CreateSemaphore(NULL, 0, 1, "Print");
	HANDLE reset = CreateSemaphore(NULL, 0, 1, "Reset");
	HANDLE close = CreateSemaphore(NULL, 0, 1, "Close");
	HANDLE access = CreateMutex(NULL, FALSE, "Access");

	HANDLE controller = CreateThread(NULL, 0, controller_main, NULL, 0, NULL);

	ReleaseSemaphore(print, 1, NULL);

	while (1)
	{
		if (WaitForSingleObject(reset, 10) == WAIT_OBJECT_0)
		{
			strncpy(server_buffer, (char*)buff, 256);
			for (int i = 0; i < strlen(server_buffer); ++i)
			{
				cout << server_buffer[i];
				Sleep(10);
			}
			cout << endl;
			ReleaseSemaphore(print, 1, NULL);
		}

		if (WaitForSingleObject(close, 10) == WAIT_OBJECT_0)
		{
			CloseHandle(print);
			CloseHandle(reset);
			CloseHandle(close);
			WaitForSingleObject(controller, INFINITE);
			return 0;
		}

		ReleaseMutex(reset);
	}
	return 0;
}