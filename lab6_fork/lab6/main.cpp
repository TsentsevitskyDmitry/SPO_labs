#include <iostream>
#include <deque>
#include "memlib.h"

using namespace std;

void test()
{
	cout << "sizeof(GDT_RECORD) = " << sizeof(GDT_RECORD) << endl;
	cout << "sizeof(LDT_RECORD) = " << sizeof(LDT_RECORD) << endl;
	cout << "MEMORY_OFFSET = " << MEMORY_OFFSET << endl;

	init();
	char* lib_memory = (char*)get_mem();

	int barrd1 = bmalloc(3 * PAGE_SIZE);
	int barrd2 = bmalloc(5 * PAGE_SIZE);
	brealloc(barrd1, 7 * PAGE_SIZE);
	int barrd3 = bmalloc(10 * PAGE_SIZE);
	show_pages_stat();

	bfree(barrd1);
	show_pages_stat();

	int barrd4 = bmalloc(5 * PAGE_SIZE);
	show_pages_stat();

	brealloc(barrd4, 8 * PAGE_SIZE);
	show_pages_stat();

	bfree(barrd1);
	bfree(barrd2);
	bfree(barrd3);
	bfree(barrd4);
	show_pages_stat();
	cout << "press enter";
	cin.get();
}

void show_error()
{
	cout << "Error code is: " << get_error_code() << endl;
	cout << "press enter";
	cin.get();
}

char user_choise() 
{
	char choise;
	do {
		choise = 0;
		cout << "input: ";
		cin >> choise;
		cin.clear();
		cin.ignore();
	} while (choise < '0' && choise > '9' && choise != 'q');
	return choise;
}

int user_int()
{
	int uint;
	uint = 0;
	cin >> uint;
	cin.clear();
	cin.ignore();
	return uint;
}

int user_choise_int(int max)
{
	int uint;
	do {
		uint = 0;
		cin >> uint;
		cin.clear();
		cin.ignore();
	} while (uint < 0 && uint > max);
	return uint;
}

void show_alloc_mem(deque<int> &alloc_mem)
{
	int n = 0;
	for (auto i : alloc_mem)
	{
		cout << "Adress #" << n << " = " << i << endl;
		++n;
	}
}

int main() 
{
	int index, size, new_malloc_addr, user_addr, byte;
	deque<int> alloc_mem;

	while (1)
	{
		cout << "Check option:" << endl;
		cout << "1) malloc some memory" << endl;
		cout << "2) realloc some memory" << endl;
		cout << "3) free some memory" << endl;
		cout << "4) show memory map" << endl;
		cout << "5) run test" << endl;
		cout << "6) rw byte" << endl;
		cout << "q) quit" << endl;

		switch (user_choise())
		{
		case '1':
			show_alloc_mem(alloc_mem);
			cout << "Enter size in bytes: ";
			new_malloc_addr = bmalloc(user_int());
			if (new_malloc_addr == -1)
				show_error();
			else
				alloc_mem.push_back(new_malloc_addr);
			break;

		case '2':
			show_alloc_mem(alloc_mem);
			cout << "Enter index: ";
			index = user_choise_int(alloc_mem.size());
			cout << "Enter size in bytes: ";
			size = user_int();
			brealloc(alloc_mem[index], size);
			if (new_malloc_addr == -1)
				show_error();
			break;

		case '3':
			show_alloc_mem(alloc_mem);
			cout << "Enter index: ";
			index = user_choise_int(alloc_mem.size());
			bfree(alloc_mem[index]);
			alloc_mem.erase(alloc_mem.begin() + index);
			break;

		case '4':
			puts("Pages flags");
			show_pages_stat();
			cout << "press enter";
			cin.get();
			break;

		case '5':
			test();
			break;

		case '6':
			show_alloc_mem(alloc_mem);
			cout << "Enter address: ";
			user_addr = user_int();
			if (!write_byte(user_addr, 0xA5))
				show_error();
			cout << "writed: 0xA5, readed: " << (int)read_byte(user_addr) << endl;
			cout << "press enter";
			cin.get();
			break;

		case 'q':
			return 0;
		}
		system("cls");
	}
	return 0;
}