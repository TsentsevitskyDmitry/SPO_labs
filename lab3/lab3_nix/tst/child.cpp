#define _CRT_SECURE_NO_WARNINGS
#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <iostream>
#include <string.h>
#include "esemaphore.h"

using namespace std;


int main(int argc, char* argv[])
{
	char* server_buffer = "in print ";
	int sem = atoi(argv[1]);

	const int print_key = 666;
	int print = CreateSemaphore(print_key + sem);
	int reset = CreateSemaphore('r');
	cout << "sem print = " << print << endl;;
	cout << "sem reset = " << reset << endl;;

	while (true)
	{
		if (WaitSemaphore(print, 0) == 0)
		{
			usleep(100000);
			cout << "print now = " << GetValSemaphore(print) << " ";

			for (int i = 0; i < strlen(server_buffer); ++i)
			{
				cout << server_buffer[i] << flush;
				usleep(50000);
			}
			cout << sem;
			ReleaseSemaphore(reset, 0);
			cout << "reset val = " << GetValSemaphore(reset) << endl;
		}
		usleep(100000);
	}
	return 0;
}


