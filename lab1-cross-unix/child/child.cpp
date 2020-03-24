#ifdef __linux__
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#elif _WIN32 | _WIN64
#include <conio.h>
#include <windows.h>
#endif

#include <iostream>
#include "Storage.h"
using namespace std;

int main(int argc, char* argv[])
{
	Storage storage;

	if (argc != 3) 
	{
		cout << "Settings mode" << endl;
		for (int i = 0; i < 4; ++i)
		{
			Item tmp;
			cout << "Item #" << i << endl;
			cout << "price: "; cin >> tmp.price;
			cout << "count: "; cin >> tmp.count;

			storage.setItem(tmp, i);
		}
		ofstream file("./storage.txt");
		file << storage;
		file.close();
		return 0;
	}

	ifstream ifile("storage.txt");
	ifile >> storage;
	ifile.close();

	int coins = atoi(argv[1]);
	int itemIndex = atoi(argv[2]) - 1;

	cout << "Press enter to send response c" << endl;
	cin.get();

	if (!storage.checkAvailability(itemIndex))
		return 1;

	if (!storage.checkPrice(itemIndex, coins))
		return 2;

	ofstream ofile("storage.txt");
	ofile << storage;
	ofile.close();
	return 0;
}