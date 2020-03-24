#include <iostream>
#include "memlib.h"

using namespace std;

void stat(char* lib_memory)
{
	cout << "************************************" << endl;
	cout << "free pages = " << (*((short int*)lib_memory)) << endl;
	cout << "GDT state = " << (*(short int*)(GDT_OFFSET + lib_memory)) << endl;
	cout << "LDT address = " << ((GDT_RECORD*)(GDT_OFFSET + lib_memory))[1].ldt_address << endl;
	cout << "************************************" << endl;
}

void full_stat(char* lib_memory)
{
	cout << "************************************" << endl;
	cout << "free pages = " << (*((short int*)lib_memory)) << endl;
	cout << "GDT state = " << (*(short int*)(GDT_OFFSET + lib_memory)) << endl;
	cout << "LDT address = " << ((GDT_RECORD*)(GDT_OFFSET + lib_memory))[1].ldt_address << endl;
	cout << "LDT count = " << *((short int*)(((GDT_RECORD*)(GDT_OFFSET + lib_memory))[1].ldt_address + lib_memory)) << endl;
	cout << "************************************" << endl;
}


int main() 
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

	bfree(barrd2);
	show_pages_stat();

	int barrd4 = bmalloc(5 * PAGE_SIZE);
	show_pages_stat();

	bfree(barrd1);
	bfree(barrd2);
	bfree(barrd3);
	bfree(barrd4);
	//show_pages_stat();
	cin.get();
	return 0;
}