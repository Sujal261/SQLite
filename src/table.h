#pragma once
#include<stdint.h>
#define PAGE_SIZE  4096
#define TABLE_MAX_PAGES  100
#include"row.h"
#define ROWS_PER_PAGE  (PAGE_SIZE/ROW_SIZE)
#define TABLE_MAX_ROWS  (ROWS_PER_PAGE * TABLE_MAX_PAGES)

typedef struct Table{
    uint32_t num_rows; 
    void* pages[TABLE_MAX_PAGES];
}Table;

Table* new_table();

void free_table(Table* table);