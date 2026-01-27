#pragma once
#include<stdint.h>
#include"row.h"

#define PAGE_SIZE  4096
#define TABLE_MAX_PAGES  100
#define LEAF_NODE_RIGHT_SPLIT (LEAF_NODE_MAX_CELLS+1)/2
#define LEAF_NODE_LEFT_SPLIT (LEAF_NODE_MAX_CELLS+1)-LEAF_NODE_RIGHT_SPLIT
//Here we are defining the header for the internal node 

#define INTERNAL_NODE_NUM_KEYS_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_NUM_KEYS_OFFSET COMMON_NODE_HEADER_SIZE
#define INTERNAL_NODE_RIGHT_CHILD_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_RIGHT_CHILD_OFFSET (INTERNAL_NODE_NUM_KEYS_OFFSET+INTERNAL_NODE_NUM_KEYS_SIZE)
#define INTERNAL_NODE_HEADER_SIZE (COMMON_NODE_HEADER_SIZE+INTERNAL_NODE_NUM_KEYS_SIZE+INTERNAL_NODE_RIGHT_CHILD_SIZE)

//here we are definin the body of internal node

#define INTERNAL_NODE_KEY_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_CHILD_SIZE sizeof(uint32_t)
#define INTERNAL_NODE_CELL_SIZE (INTERNAL_NODE_KEY_SIZE+INTERNAL_NODE_CHILD_SIZE)
#define INTERNAL_NODE_MAX_CELLS 3
#define INVALID_PAGE_NUM UINT32_MAX

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
uint32_t get_unused_page_num(Pager* pager);
void create_new_root_node(Table* table, uint32_t right_child_page_num);

uint32_t* internal_node_num_keys(void* node);
uint32_t* internal_node_child(void* node, uint32_t child_num);
uint32_t* internal_node_key(void* node, uint32_t key_num);
uint32_t* internal_node_right_child(void* node);
uint32_t  get_node_max_key(void* node);
bool is_root_node(void* node); 
uint32_t* internal_node_cell(void* node, uint32_t cell_num);
void set_root_node(void* node, bool is_root);
uint32_t* node_parent(void* node);
void update_internal_node_key(void* node, uint32_t old_key, uint32_t new_key);
void internal_node_insert(Table* table, uint32_t parent_page_num, uint32_t child_page_num);
uint32_t internal_node_find_child(void* node, uint32_t key);
void internal_node_split_and_insert(Table* table, uint32_t parent_page_num, uint32_t child_page_num);