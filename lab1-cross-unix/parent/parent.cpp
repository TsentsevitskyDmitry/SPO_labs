#include <iostream>
#include "VendingMachine.h"

int main()
{
#ifdef _WIN32 | _WIN64
	system("chcp 1251 > nul");
#endif
    
	VendingMachine machine;
	machine.showVendingMenu();

	return 0;
}
