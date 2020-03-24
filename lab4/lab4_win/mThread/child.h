#define _CRT_SECURE_NO_WARNINGS
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

typedef struct {
	string syncName;
	string unicString;
} ChildArgs;

DWORD WINAPI child_main(LPVOID lpParam);