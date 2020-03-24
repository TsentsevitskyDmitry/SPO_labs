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
	if (argc != 4) return -1;
	int sem = atoi(argv[1]);
	int pipe_write = atoi(argv[2]);
	char client_string[256];
	strcpy(client_string, argv[3]);

	const int print_key = 666;
	const int close_key = 6666;
	int print = CreateSemaphore(print_key + sem);
	int close = CreateSemaphore(close_key + sem);
	int reset = CreateSemaphore('r');


	while (true)
	{
		if (WaitSemaphore(print, 0) == 0)
		{
			write(pipe_write, client_string, 256);
			ReleaseSemaphore(reset, 0);
		}
		usleep(100000);

		if (WaitSemaphore(close, 0) == 0)
		{
			DisposeSemaphore(print);
			DisposeSemaphore(close);
			return 0;
		}
		usleep(100000);
	}
	return 0;
}


