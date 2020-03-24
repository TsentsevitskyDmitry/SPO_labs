#pragma once
// memory:
// ## free pages count : pages flags ## GDT len : GDT ## other memory 
// ## PAGES_FIELD_SIZE :  1024 bytes ##    GDT_SIZE   ## other memory 


#pragma pack(push, 1)

struct GDT_RECORD // 48 bit
{
	int ldt_address;
	short int size;
};

struct LDT_RECORD // 32 bit 
{
	int page_address;
};
#pragma pack(pop)

// MEMORY CONSTANTS 
const int GDT_FLAG = 0b00000000;
const int LDT_FLAG = 0b00000001;
const int FREE_MEM_FLAG = 0b00000010;
const int USED_MEM_FLAG = 0b00000011;

// ERROR CODES
const int GDT_OVERFLOW = 0x00;
const int OUT_OF_MEMORY = 0x01;
const int WRONG_ADDRESS = 0x02;
const int WRONG_SIZE = 0x03;


// MEMORY SETTINGS
const int PAGE_SIZE = 256 * 256; // 64 Kb
const int MEMORY_SIZE = PAGE_SIZE * 1024; // 64 Mb
const int PAGES_COUNT = MEMORY_SIZE / PAGE_SIZE; // 1024
const int PAGES_COUNT_FIELD_SIZE = 2; // 256 * 256 < 1024

// MEMORY STRUCTURE SETTINGS
const int GDT_SIZE = 256;
const int LDT_SIZE = sizeof(LDT_RECORD) * PAGES_COUNT + 2; // 4 kb
const int GDT_OFFSET = PAGES_COUNT_FIELD_SIZE + PAGES_COUNT;

// MEMORY ADDRESSING
static void* memory;
const int MEMORY_OFFSET = (PAGES_COUNT_FIELD_SIZE + PAGES_COUNT + sizeof(GDT_RECORD) * GDT_SIZE + (PAGE_SIZE - 1)) / PAGE_SIZE;

static int error_code;


void init();
void* get_mem();

short int get_next_free_page(int from);
void set_page_flag(int from, size_t size, int flag);
int alloc_ldt(size_t size);

int bmalloc(size_t size);
void bfree(int vaddress);
void brealloc(int vaddress, size_t size);

void show_pages_stat();