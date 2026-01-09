#pragma once
#define PAGE_SIZE  4096
#define TABLE_MAX_PAGES  100
#include"row.h"
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

Table* new_table();

void free_table(Table* table);