#pragma once
#include "Menu.h"

class VendingMachine
{
private:

#ifdef __linux__
	const char* corePath = "./child_e";
#elif _WIN32 | _WIN64
	const char* corePath = "child.exe";
#endif

	Menu mainMenu, choiseMenu;
	int inCoins;
	int processBackground(int itemIndex);

public:
	VendingMachine();
	void showVendingMenu();
	void ConstRequest();
};

