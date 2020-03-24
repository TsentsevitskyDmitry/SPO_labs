#pragma once
#include <fstream>

using namespace std;

struct Item
{
	int count;
	int price;
};

class Storage
{
private:
	Item items[4] = {};

public:
	void setItem(Item& item, int itemIndex);

	bool checkAvailability(int itemIndex);
	bool checkPrice(int itemIndex, int money);
	void cell(int itemIndex);

	friend ofstream& operator<< (ofstream& out, Storage& st);
	friend ifstream& operator>> (ifstream& in, Storage& st);
};

