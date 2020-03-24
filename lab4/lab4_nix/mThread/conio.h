#pragma once
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;

void clearConsole();

int _getch();
void enable_raw_mode();
void disable_raw_mode();
bool _kbhit();