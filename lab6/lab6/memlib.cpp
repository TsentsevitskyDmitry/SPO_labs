#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memlib.h"

void* get_mem() { return memory; }

void init()
{
	memory = malloc(MEMORY_SIZE);
	memset(memory, 0, MEMORY_OFFSET * PAGE_SIZE);

	set_page_flag(0, MEMORY_OFFSET, GDT_FLAG);
	set_page_flag(MEMORY_OFFSET, PAGES_COUNT - MEMORY_OFFSET, FREE_MEM_FLAG);

	short int* free_pages = (short int*)memory;
	*free_pages = 1024 - MEMORY_OFFSET;
}

void set_error_code(int code)
{
	error_code = code;
}

short int get_next_free_page(int from)
{
	char* page_flags = PAGES_COUNT_FIELD_SIZE + (char*)memory;
	for (short int i = from; i < PAGES_COUNT; ++i) 
	{
		if (page_flags[i] == FREE_MEM_FLAG)
			return i;
	}

	return -1;
}

void set_page_flag(int from, size_t size, int flag)
{
	void* from_page = PAGES_COUNT_FIELD_SIZE + (char*)memory + from;
	memset(from_page, flag, size);
}

bool check_size(size_t size) 
{
	if (size < 1)
	{
		set_error_code(WRONG_SIZE);
		return false;
	}
}

bool check_memory(size_t size)
{
	short int* free_pages = (short int*)memory;
	if (*free_pages < size)
	{
		set_error_code(OUT_OF_MEMORY);
		return false;
	}
	return true;
}

bool check_address(int vaddress) 
{
	GDT_RECORD* GDT = (GDT_RECORD*)(GDT_OFFSET + (char*)memory);
	int ldt_selector = vaddress >> 16;
	if (GDT[ldt_selector].ldt_address == 0) 
	{
		set_error_code(WRONG_ADDRESS);
		return false;
	}
	else return true;
}

int alloc_ldt(size_t pages)
{
	short int* free_pages = (short int*)memory;
	if (*free_pages < pages + 1)
	{
		set_error_code(OUT_OF_MEMORY);
		return 0;
	}
	*free_pages -= 1;

	short int ldt_page = get_next_free_page(0);
	set_page_flag(ldt_page, 1, LDT_FLAG);
	int ldt_address = ldt_page * PAGE_SIZE;
	memset((char*)memory + ldt_address, 0, 2);
	return ldt_address;
}

void ldt_add_pages(int ldt_address, size_t pages)
{
	LDT_RECORD* LDT = (LDT_RECORD*)((char*)memory + ldt_address);

	short int* free_pages = (short int*)memory;
	*free_pages -= pages;

	short int* ldt_lenght = (short int*)LDT;
	short int mem_page = 0;

	for (int i = 1; i <= pages; ++i)
	{
		mem_page = get_next_free_page(mem_page);
		LDT[*ldt_lenght + i].page_address = mem_page * PAGE_SIZE;
		set_page_flag(mem_page, 1, USED_MEM_FLAG);
	}

	*ldt_lenght += pages;
}

void ldt_remove_pages(int ldt_address, size_t pages)
{
	LDT_RECORD* LDT = (LDT_RECORD*)((char*)memory + ldt_address);

	short int* free_pages = (short int*)memory;
	*free_pages += pages;

	short int* ldt_lenght = (short int*)LDT;
	short int mem_page = 0;

	for (int i = 0; i < pages; ++i)
	{
		set_page_flag(LDT[*ldt_lenght - i].page_address / PAGE_SIZE, 1, FREE_MEM_FLAG);
		LDT[*ldt_lenght - i].page_address = 0;
	}

	*ldt_lenght -= pages;
}

short int increase_gdt_length(GDT_RECORD* GDT)
{
	short int* gdt_length = (short int*)GDT;
	if (*gdt_length == GDT_SIZE - 1)
	{
		set_error_code(GDT_OVERFLOW);
		return 0;
	}
	*gdt_length += 1;

	for (int i = 1; i < GDT_SIZE; ++i)
	{
		if (GDT[i].ldt_address == 0) return i;
	}

	return 0;
}

int bmalloc(size_t size)
{
	if (!memory) init();
	size_t in_pages_size = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	if (!check_size(in_pages_size)) return -1;

	GDT_RECORD* GDT = (GDT_RECORD*)(GDT_OFFSET + (char*)memory);

	short int ldt_selector = increase_gdt_length(GDT);
	if (!ldt_selector) return -1;

	int ldt_address = alloc_ldt(in_pages_size);
	if (!ldt_address) return -1;

	ldt_add_pages(ldt_address, in_pages_size);

	GDT[ldt_selector].ldt_address = ldt_address;
	GDT[ldt_selector].size = in_pages_size;

	return ldt_selector << 16;
}

void ldt_clear_pages(int ldt_address)
{
	LDT_RECORD* LDT = (LDT_RECORD*)((char*)memory + ldt_address);
	short int* ldt_lenght = (short int*)LDT;

	short int* free_pages = (short int*)memory;
	*free_pages += *ldt_lenght;

	for (int i = 1; i <= *ldt_lenght; ++i)
	{
		int page_address = LDT[i].page_address;
		set_page_flag(page_address / PAGE_SIZE, 1, FREE_MEM_FLAG);
	}
}

void free_ltd(int ldt_address)
{
	short int* free_pages = (short int*)memory;
	*free_pages += 1;
	short int ldt_page = ldt_address / PAGE_SIZE;
	set_page_flag(ldt_page, 1, FREE_MEM_FLAG);
}

void decrease_gdt_length(GDT_RECORD* GDT)
{
	short int* gdt_length = (short int*)GDT;
	*gdt_length -= 1;
}

void bfree(int vaddress)
{
	if (!memory) return;
	if (!check_address(vaddress)) return;
	GDT_RECORD* GDT = (GDT_RECORD*)(GDT_OFFSET + (char*)memory);

	int ldt_selector = vaddress >> 16;
	int ldt_address = GDT[ldt_selector].ldt_address;
	ldt_clear_pages(ldt_address);
	free_ltd(ldt_address);
	decrease_gdt_length(GDT);
	GDT[ldt_selector].ldt_address = 0;
	GDT[ldt_selector].size = 0;
}


void brealloc(int vaddress, size_t size)
{
	if (!memory) return;
	if (!check_address(vaddress)) return;
	GDT_RECORD* GDT = (GDT_RECORD*)(GDT_OFFSET + (char*)memory);
	short int new_in_pages_size = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	if (!check_size(new_in_pages_size)) {
		bfree(vaddress);
		return;
	}

	int ldt_selector = vaddress >> 16;
	if (GDT[ldt_selector].size < new_in_pages_size)
	{
		if (!check_memory(new_in_pages_size)) return;
		ldt_add_pages(GDT[ldt_selector].ldt_address, new_in_pages_size - GDT[ldt_selector].size);
	}
	else
	{
		ldt_remove_pages(GDT[ldt_selector].ldt_address, GDT[ldt_selector].size - new_in_pages_size);
	}

	GDT[ldt_selector].size = new_in_pages_size;
}

void show_pages_stat() 
{
	puts("Pages flags");
	char* page_flags = PAGES_COUNT_FIELD_SIZE + (char*)memory;

	for (int i = 0; i < 960; ++i)
	{
		char c;
		switch (page_flags[i])
		{
		case GDT_FLAG:
			c = 'G';
			break;
		case LDT_FLAG:
			c = 'L';
			break;;
		case FREE_MEM_FLAG:
			c = '_';
			break;;
		case USED_MEM_FLAG:
			c = '#';
			break;;
		}
		putchar(c);
	}
	puts("");
}