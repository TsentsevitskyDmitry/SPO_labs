#pragma once
#include <string> 
#include <vector> 

using namespace std;

class Menu
{
private:
	string head;
	vector<string> items;
	int position;
	void process();

public:
	Menu();
	~Menu();

	void setHead(string text);
	void addItem(string name);
	int getChoise();
	string getItem(int index);
	int lastIndex();
	int choise();
};

