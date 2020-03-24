#pragma once
#include <Windows.h>

struct RWSettings
{
	HANDLE hFile;
	DWORD bufferLen;
	LPVOID buffer;
	OVERLAPPED overlapped;
	DWORD RWBytes;
	HANDLE successEvent;
};

extern "C" __declspec(dllexport) DWORD result(RWSettings *settings);
extern "C" __declspec(dllexport) void read(RWSettings *settings);
extern "C" __declspec(dllexport) void write(RWSettings *settings);