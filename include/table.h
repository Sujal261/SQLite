#pragma once
#include<stdint.h>
#define PAGE_SIZE  4096
#define TABLE_MAX_PAGES  100
#define ROWS_PER_PAGE  (PAGE_SIZE/ROW_SIZE)
#define TABLE_MAX_ROWS  (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct{
    int file_descriptor; 
    uint32_t file_length; 
    void* pages[TABLE_MAX_PAGES];
}Pager;

typedef struct Table{
    uint32_t num_rows; 
    Pager* pager;
}Table;

void db_close(Table* table);
Table* db_open(const char* filename);
Pager* pager_open(const char* filename);
void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);
void free_table(Table* table);