#define _CRT_SECURE_NO_WARNINGS
#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include "Storage.h"
using namespace std;


int main(int argc, char* argv[])
{
	if (argc != 3) return -1;
	string sem = string(argv[1]);
	char client_string[256];
	strcpy(client_string, argv[2]);

	HANDLE print = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, string("PrintController").append(sem).c_str());
	HANDLE close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, string("CloseController").append(sem).c_str());
	HANDLE reset = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Reset");

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
		if (WaitForSingleObject(print, 10) == WAIT_OBJECT_0)
		{
			strncpy((char*)buff, client_string, 256);
			ReleaseSemaphore(reset, 1, NULL);
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


int old_main(int argc, char* argv[])
{
	Storage storage;

	if (argc != 3) 
	{
		cout << "Settings mode" << endl;
		for (int i = 0; i < 4; ++i)
		{
			Item tmp;
			cout << "Item #" << i << endl;
			cout << "price: "; cin >> tmp.price;
			cout << "count: "; cin >> tmp.count;

			storage.setItem(tmp, i);
		}
		ofstream file("./storage.txt");
		file << storage;
		file.close();
		return 0;
	}

	ifstream ifile("storage.txt");
	ifile >> storage;
	ifile.close();

	int coins = atoi(argv[1]);
	int itemIndex = atoi(argv[2]) - 1;

	cout << "Press enter to send response c" << endl;
	cin.get();

	if (!storage.checkAvailability(itemIndex))
		return 1;

	if (!storage.checkPrice(itemIndex, coins))
		return 2;

	ofstream ofile("storage.txt");
	ofile << storage;
	ofile.close();
	return 0;
}