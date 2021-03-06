#pragma once
#ifdef __linux__
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <iostream>
using namespace std;

void clearConsole();

class ExtProcess
{
private:
#ifdef __linux__
    pid_t pid;
    int st;
#elif _WIN32 | _WIN64
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
#endif

public:
	ExtProcess();
	~ExtProcess();

	void createProcess(string path, string args, bool createNewConsole);
	int wait(int timeout);
	int getExitCode();
};
