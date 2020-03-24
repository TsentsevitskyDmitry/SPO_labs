#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <vector>
#include "extprocess.h"
#include "child.h"

using namespace std;

void pushWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder, string unicString);
void popWriter(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);
void disposeWriters(vector<HANDLE> &writeOrder, vector<HANDLE> &closeOrder);

DWORD WINAPI controller_main(LPVOID lpParam);