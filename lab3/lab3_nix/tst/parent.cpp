#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <wait.h>
#include <semaphore.h>
//#include "VendingMachine.h"
#include "extprocess.h"
#include "esemaphore.h"

const char* corePath = "./controller_e";


int main()
{
#ifdef _WIN32 | _WIN64
	system("chcp 1251 > nul");
#endif
	//char server_buffer[256];
	char* server_buffer = "testn\n";
	int print = CreateSemaphore('p');
	int reset = CreateSemaphore('r');
	int sclose = CreateSemaphore('c');

	int pipefd[2];
	if (pipe(pipefd) == -1)
		return -1;
	int pipe_pread = pipefd[0];
	int pipe_write = pipefd[1];

	cout << "parent write pipe = " << pipe_write << endl;
	cout << "parent read pipe = " << pipe_pread << endl;


	ExtProcess controller;
	controller.createProcess(corePath, to_string(pipe_write), true);
	cout << "Controller started." << endl;
	ReleaseSemaphore(print, 0);
	cout << "Server is runing." << endl << endl;
	//WaitSemaphore(reset, 0);

	while (1)
	{
		if (WaitSemaphore(reset, 0) == 0)
		{
			//read(pipe_pread, &server_buffer, 256);
			for (int i = 0; i < strlen(server_buffer); ++i)
			{
				cout << server_buffer[i] << flush;
				usleep(50000);
			}
			cout << endl;

			if (strlen(server_buffer))
				ReleaseSemaphore(print, 0);
		}

		if (WaitSemaphore(sclose, 0) == 0)
		{
			cout << "in cl";
			controller.wait(1000000);
			close(pipe_pread);
			close(pipe_write);
			DisposeSemaphore(print);
			DisposeSemaphore(reset);
			DisposeSemaphore(sclose);
			return 0;
		}
	}

	//VendingMachine machine;
	//machine.ConstRequest();

	return 0;
}
