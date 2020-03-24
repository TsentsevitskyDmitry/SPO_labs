#define _CRT_SECURE_NO_WARNINGS
#ifdef __linux__
#include <stdio.h>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <iostream>
#endif

#include <ctime>
#include "VendingMachine.h"
#include "extprocess.h"
using namespace std;

VendingMachine::VendingMachine()
{
	mainMenu.setHead("Вы стоите перед Вендинг Машиной:");
	mainMenu.addItem("Сделать выбор");
	mainMenu.addItem("Опустить деньги");
	mainMenu.addItem("Отправиться в путь");

	choiseMenu.setHead("Выбор:");
	choiseMenu.addItem("Мечь");
	choiseMenu.addItem("Щит");
	choiseMenu.addItem("Лук");
	choiseMenu.addItem("Стрелы");

	inCoins = 0;
}

void VendingMachine::showVendingMenu()
{
	while (mainMenu.getChoise() != mainMenu.lastIndex())
	{
		switch (mainMenu.choise())
		{
		case 1:
			choiseMenu.getChoise();
			processBackground(choiseMenu.choise());
			cin.get();
			break;

		case 2:
			clearConsole();
			cout << "Всего опущено: " << this->inCoins << "(Сдачи не выдает)" << endl;
			int coins;
			do
			{
				cout << "Вы опускаете: ";
				cin >> coins;
				cin.get();
				cin.clear();
				rewind(stdin);
			} while (!cin.good());

			this->inCoins += coins;
			break;
		}
	}
}

void VendingMachine::ConstRequest()
{
	this->inCoins = 100;
	processBackground(1);

}


int VendingMachine::processBackground(int itemIndex)
{
	ExtProcess ext;
	char args[256];
#ifdef __linux__
	snprintf(args, 256, "%d %d", this->inCoins, itemIndex);
#elif _WIN32 | _WIN64
	_snprintf_s(args, 256, "%d %d", this->inCoins, itemIndex);
#endif

	ext.createProcess(this->corePath, args, true);

	time_t ltime;
	while (ext.wait(50))
	{
		clearConsole();
		time(&ltime);
		cout << ctime(&ltime) << (char)10 << endl;
	}

	clearConsole();
	switch (ext.getExitCode())
	{
	case 0:
		cout << "Вы получили: " + this->choiseMenu.getItem(itemIndex) << "!" << endl;
		this->inCoins = 0;
		break;

	case 1:
		cout << "Все раскуплено: " + this->choiseMenu.getItem(itemIndex) << "!" << endl;

		break;

	case 2:
		cout << "Опущено недостаточно!" << endl;
		break;
	}

	return 0;
}