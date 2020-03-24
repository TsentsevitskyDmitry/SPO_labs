#include "Storage.h"

void Storage::setItem(Item& item, int itemIndex)
{
	items[itemIndex] = item;
}

bool Storage::checkAvailability(int itemIndex)
{
	return items[itemIndex].count != 0;
}

bool Storage::checkPrice(int itemIndex, int money)
{
	return items[itemIndex].price <= money;
}

void Storage::cell(int itemIndex)
{
	items[itemIndex].count--;
}

ifstream & operator>>(ifstream & in, Storage & st)
{
	Storage tmp;
	in.read((char*)&tmp, sizeof(Storage));
	st = tmp;
	return in;
}

ofstream & operator<<(ofstream & out, Storage & st)
{
	out.write((char*)&st, sizeof(Storage));
	return out;
}
