#ifdef __linux__
#include <termios.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include "extprocess.h"

#ifdef __linux__
int _getch()
{
	struct termios oldt, newt;
	int ch;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}
#endif

void clearConsole()
{
#ifdef __linux__
    std::system ("clear");
#elif _WIN32 | _WIN64
    system("cls");       
#endif
}

ExtProcess::ExtProcess()
{
#ifdef __linux__

#elif _WIN32 | _WIN64
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
#endif
}

ExtProcess::~ExtProcess()
{
#ifdef __linux__

#elif _WIN32 | _WIN64
	CloseHandle(pi.hProcess);
#endif
}

void ExtProcess::createProcess(string path, string args, bool createNewConsole)
{
	path = path + " " + args;

#ifdef __linux__
	pid = fork();
	switch(pid)
	{
		case -1: {
			cout << "Error -1." << endl;
			break;
		}

		case 0: {
			if(execlp("./run.sh", "./", path.c_str(), NULL) == -1)
				cout << "Error: " << path << endl;
			break;
		}
	}

#elif _WIN32 | _WIN64
	DWORD console = NULL;
	if (createNewConsole)
		console = CREATE_NEW_CONSOLE;

	if (!CreateProcess(NULL,	// No module name (use command line)
		TEXT((LPSTR)path.c_str()),		// Command line
		NULL,					// Process handle not inheritable
		NULL,					// Thread handle not inheritable
		FALSE,					// Set handle inheritance to FALSE
		console,				// lol
		NULL,					// Use parent's environment block
		NULL,					// Use parent's starting directory 
		&si,					// Pointer to STARTUPINFO structure
		&pi)					// Pointer to PROCESS_INFORMATION structure
		)
	{
		printf("CreateProcess failed (%d)\n", GetLastError());
		return;
	}
#endif
}

int ExtProcess::wait(int timeout)
{
#ifdef __linux__
    if(waitpid(pid,&st,WNOHANG)>0)
	   return 0;
	usleep(timeout*1000);
	return 1;
#elif _WIN32 | _WIN64
	return WaitForSingleObject(pi.hProcess, timeout);
#endif
}

int ExtProcess::getExitCode()
{
#ifdef __linux__
    return WEXITSTATUS(st); 
#elif _WIN32 | _WIN64
	DWORD result;
	GetExitCodeProcess(pi.hProcess, &result); 
	return result;
#endif
}
