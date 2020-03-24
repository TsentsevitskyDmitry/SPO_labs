// Mylab5.cpp : Defines the entry point for the console application.
#include <windows.h>
#include <process.h>
#include <iostream>
#include <string>
#include "./../library/rwlib.h"

using namespace std;

typedef struct asyncBuffer
{
	char* buffer;
	int bytes;
} asyncBuffer;

DWORD WINAPI reader(PVOID argv);
DWORD WINAPI writer(PVOID pvParam);
void handleError();

HINSTANCE RWlib;
const int MAX_READ_LEN = 32;
const char* OUTPUT_FILE_PATH = "./out.txt";

int main(int argc, char *argv[])
{
	RWlib = LoadLibrary("rwlib.dll");
	if (!RWlib) handleError();

	asyncBuffer aBuff;
	aBuff.buffer = new char[MAX_READ_LEN];

	HANDLE readComplete = CreateEvent(NULL, FALSE, FALSE, TEXT("lab5_read_complete"));
	HANDLE writeComplete = CreateEvent(NULL, FALSE, TRUE, TEXT("lab5_write_complete"));

	HANDLE hThreads[2];   // Дескрипторы потока-писателя и потока-читателя

	hThreads[0] = CreateThread(NULL, 0, reader, (asyncBuffer*)&aBuff, 0, NULL);
	hThreads[1] = CreateThread(NULL, 0, writer, (asyncBuffer*)&aBuff, 0, NULL);

	WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	CloseHandle(readComplete);
	CloseHandle(writeComplete);

	FreeLibrary(RWlib);
	cin.ignore();
	cin.get();
	return 0;
}

void handleError()
{
	cout << "Error: " << GetLastError() << endl;
	cin.ignore();
	cin.get();
	exit(-1);
}

DWORD WINAPI reader(PVOID commonBuffer)
{
	DWORD(*Result)(RWSettings*) = (DWORD(*)(RWSettings*))GetProcAddress(RWlib, "result");
	void(*Read)(RWSettings*) = (void(*)(RWSettings*))GetProcAddress(RWlib, "read");

	asyncBuffer* aBuff = (asyncBuffer*)commonBuffer;
	aBuff->bytes = 0;
	RWSettings Rsettings;
	ZeroMemory(&Rsettings, sizeof(RWSettings));
	Rsettings.buffer = aBuff->buffer;
	Rsettings.bufferLen = MAX_READ_LEN;
	Rsettings.successEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("lab5_lib_read_event"));
	HANDLE readComplete = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("lab5_read_complete"));
	HANDLE writeComplete = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("lab5_write_complete"));

	string folder;
	cout << "Input directory with files to combine: ";
	cin >> folder;
	folder.append("\\");
	string searchMask = folder + "*.txt";

	char sourceFilePath[MAX_PATH];
	WIN32_FIND_DATA FindedFile;
	HANDLE searchHandle;

	searchHandle = FindFirstFile(searchMask.c_str(), &FindedFile);
	if (searchHandle == INVALID_HANDLE_VALUE)
		handleError();

	while (1)
	{
		while (WaitForSingleObject(writeComplete, 100) != WAIT_OBJECT_0)
		{	;	} // что-то очень асинхронно делаем

		if (!aBuff->bytes)
		{
			strcpy(sourceFilePath, folder.c_str());
			strcat(sourceFilePath, FindedFile.cFileName);  // Получаем путь к текущему читаемому файлу
			Rsettings.hFile = CreateFile(sourceFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			cout << "Opening file: " << sourceFilePath << endl;
		}
		Read(&Rsettings);	// Прочитать из файла

		while (WaitForSingleObject(Rsettings.successEvent, 100) != WAIT_OBJECT_0) 
		{	;	} // снова что-то очень асинхронно делаем
		aBuff->bytes = Result(&Rsettings);

		if (!aBuff->bytes) // читать больше нечего
		{
			CloseHandle(Rsettings.hFile);
			if (!FindNextFile(searchHandle, &FindedFile)) 
			{
				aBuff->bytes = -1;
				SetEvent(readComplete);
				break;
			}
			SetEvent(writeComplete);
		}
		else
			SetEvent(readComplete);
	}

	FindClose(searchHandle);
	CloseHandle(readComplete);
	CloseHandle(writeComplete);
	CloseHandle(Rsettings.successEvent);
	return 0;
}

DWORD WINAPI writer(PVOID commonBuffer)
{
	void(*Write)(RWSettings*) = (void(*)(RWSettings*))GetProcAddress(RWlib, "write");

	asyncBuffer *aBuff = (asyncBuffer*)commonBuffer;
	RWSettings Wsettings;
	ZeroMemory(&Wsettings, sizeof(RWSettings));
	Wsettings.buffer = aBuff->buffer;
	Wsettings.successEvent = CreateEvent(NULL, FALSE, TRUE, TEXT("lab5_write_event"));
	Wsettings.hFile = CreateFile(OUTPUT_FILE_PATH, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

	HANDLE readComplete = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("lab5_read_complete"));
	HANDLE writeComplete = OpenEvent(EVENT_ALL_ACCESS, FALSE, TEXT("lab5_write_complete"));

	while(1)
	{   
		while (WaitForSingleObject(readComplete, 100) != WAIT_OBJECT_0)
		{	;	} // снова что-то очень асинхронно делаем

		if (aBuff->bytes == -1) // пора закругляться
			break;

		Wsettings.bufferLen = aBuff->bytes;
		Write(&Wsettings);

		SetEvent(writeComplete);
	}

	CloseHandle(readComplete);
	CloseHandle(writeComplete);
	CloseHandle(Wsettings.hFile);
	return 0;
}