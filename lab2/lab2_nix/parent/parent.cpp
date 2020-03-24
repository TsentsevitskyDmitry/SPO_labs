#include <iostream>
#include <string>
#include <vector>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "VendingMachine.h"

using namespace std;

int writeSignal = 0;
int closeSignal = 0;

struct sigaction writeSigaction, closeSigaction;

void setWrite(int sign)
{
  writeSignal = 1;
}

void setClose(int sign)
{
  closeSignal = 1;
}


int main(int argc, char* argv[])
{   
	if (argc < 2) return -1;

	int sep_pos = string(argv[1]).find("_");
	int parentPid = stoi(string(argv[1]).substr(0, sep_pos));
	++sep_pos;
	string unicString = string(argv[1]).substr(sep_pos, string(argv[1]).length() - sep_pos);

	writeSigaction.sa_handler = setWrite;
    sigaction(SIGUSR1, &writeSigaction, NULL);

	closeSigaction.sa_handler = setClose;
    sigaction(SIGUSR2, &closeSigaction, NULL);

	kill(parentPid, SIGUSR2);

	while (1)
	{
		if (writeSignal)
		{
			for (int i = 0; i < unicString.length(); ++i)
			{
				cout << unicString[i] << flush;
				usleep(50000);
			}
			cout << endl;
			kill(parentPid, SIGUSR1);
			writeSignal = 0;
		}

		if (closeSignal)
		{
			return 0;
		}

		usleep(100000);
	}

	//VendingMachine machine;
	//machine.ConstRequest();

	return 0;
}
