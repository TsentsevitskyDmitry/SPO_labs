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

#include "esync.h"
#include "controller.h"

int main()
{
	char server_buffer[256];
	int print = CreateSemaphore("print");
	int reset = CreateSemaphore("reset");
	int sclose = CreateSemaphore("close");
	pthread_mutex_t saccess;
	cout << pthread_mutex_init(&saccess, NULL) << endl;

	int pipefd[2];
	if (pipe(pipefd) == -1)
		return -1;
	int pipe_pread = pipefd[0];
	int pipe_write = pipefd[1];

	pthread_t controller;
	ControllerArgs args;
	args.saccess = &saccess;
	args.pipe_write = pipe_write;
	void* data = &args;
	pthread_create(&controller, NULL, controller_main, data);
	
	ReleaseSemaphore(print, 0);

	while (1)
	{
		if (WaitSemaphore(reset, 0) == 0)
		{
			//cout << "pr in rs" << endl;
			read(pipe_pread, &server_buffer, 256);
			for (int i = 0; i < strlen(server_buffer); ++i)
			{
				cout << server_buffer[i] << flush;
				usleep(10000);
			}
			cout << endl;

			ReleaseSemaphore(print, 0);
		}
		else
			usleep(10000);

		if (WaitSemaphore(sclose, 0) == 0)
		{
			pthread_join(controller, NULL);
			close(pipe_pread);
			close(pipe_write);
			DisposeSemaphore(print);
			DisposeSemaphore(reset);
			DisposeSemaphore(sclose);
			pthread_mutex_destroy(&saccess);
			return 0;
		}
		usleep(10000);

	}

	//VendingMachine machine;
	//machine.ConstRequest();

	return 0;
}
