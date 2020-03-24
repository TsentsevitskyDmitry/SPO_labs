#include "child.h"

using namespace std;


void* child_main(void* lpParam)
{
	ChildArgs* args = (ChildArgs*)lpParam;

	string sem = args->syncName;
	int pipe_write = args->pipe_write;
	pthread_mutex_t* saccess = args->saccess;
	char client_string[256];
	strcpy(client_string, args->unicString.c_str());


	//int print = CreateSemaphore(string("print").append(sem));
	int print = CreateSemaphore("print");
	int close = CreateSemaphore(string("close").append(sem));
	int reset = CreateSemaphore("reset");

	//cout << "cl rd pr = "<< ValueSemaphore(print) << " | rs = " << ValueSemaphore(reset) <<
		//"ls = " << client_string << endl;

	bool run = true;
	while (run)
	{
		if (pthread_mutex_trylock(saccess) == 0)
		{
			//cout << "cl in m pr = "<< ValueSemaphore(print) << " | rs = " << ValueSemaphore(reset) << endl;

			if (WaitSemaphore(print, 0) == 0)
			{

			//cout << "pr = "<< ValueSemaphore(print) << " | rs = " << ValueSemaphore(reset) << endl;
				write(args->pipe_write, client_string, 256);
				ReleaseSemaphore(reset, 0);

				while(ValueSemaphore(print) != 1)
					usleep(10000);
				
				//usleep(strlen(client_string) * 20000);

				usleep(10000);
				//cout << "cl un m pr = "<< ValueSemaphore(print) << " | rs = " << ValueSemaphore(reset) << endl;
			}
			else
				;//cout << "pr = "<< ValueSemaphore(print) << " | rs = " << ValueSemaphore(reset) << endl;
			
			pthread_mutex_unlock(saccess);
			usleep(10000);
		}

		if (WaitSemaphore(close, 0) == 0)
		{
			DisposeSemaphore(close);
			run = false;
		}
		usleep(100000);

	}

	pthread_mutex_unlock(saccess);
	return 0;
}


