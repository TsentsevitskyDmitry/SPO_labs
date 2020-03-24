#ifdef __linux__
#include <termios.h>
#include <cstdio>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include "Menu.h"
#include "../libs/extprocess.h"
#include <iostream>


Menu::Menu()
{
	position = 0;
}

Menu::~Menu()
{
	items.clear();
}

void Menu::setHead(string text)
{
	head = text;
}

void Menu::addItem(string name)
{
	items.push_back(name);
}

void Menu::process()
{
	while (true)
	{
		clearConsole();
		cout << head << endl;

		for (int i = 0; i < items.size(); ++i)
		{
			if (i == position)
				cout << "->";
			cout << items[i] << endl;
		}

		char key = _getch();

		if (key == 13 || key == 10)
			return;
		if (key == -32)
			key = _getch();

		if ((key == 'w' || key == 72) && position > 0)
			position--;
		if ((key == 's' || key == 80) && position < items.size() - 1)
			position++;
	}
}

int Menu::getChoise()
{
	process();
	cin.clear();
	rewind(stdin);
	return position + 1;
}

string Menu::getItem(int index)
{
	if (index < 1) return string();
	return items[index - 1];
}

int Menu::lastIndex()
{
	return items.size();
}

int Menu::choise()
{
	return position + 1;
}
