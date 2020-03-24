#ifdef __linux__
#include <termios.h>
#include <cstdio>
#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include "Menu.h"
#include "extprocess.h"
#include <iostream>


#ifdef __linux__
int getch( ) 
{
	struct termios oldt,newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}
#endif


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

		char key = getch();

		if (key == 13 || key == 10)
			return;
		if (key == -32)
			key = getch();

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
