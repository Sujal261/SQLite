#pragma once
#include<stdint.h>
#include"row.h"

#define PAGE_SIZE  4096
#define TABLE_MAX_PAGES  100

typedef struct{
    int file_descriptor; 
    uint32_t file_length; 
    uint32_t num_pages;
    void* pages[TABLE_MAX_PAGES];
}Pager;

typedef struct Table{
    Pager* pager;
    uint32_t root_page_num;
    
}Table;

void db_close(Table* table);
Table* db_open(const char* filename);
Pager* pager_open(const char* filename);
void pager_flush(Pager* pager, uint32_t page_num);
void free_table(Table* table);
void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value);
void leaf_node_split_and_insert(Cursor* cursor, uint32_t key, Row* value);
