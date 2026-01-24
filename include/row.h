#pragma once
#include<stdbool.h>
#include<stdint.h>

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define ID_SIZE  size_of_attribute(Row, id)
#define USERNAME_SIZE  size_of_attribute(Row, username)
#define EMAIL_SIZE  size_of_attribute(Row ,email)
#define ID_OFFSET  0
#define USERNAME_OFFSET  (ID_OFFSET+ID_SIZE)
#define EMAIL_OFFSET  (USERNAME_OFFSET+USERNAME_SIZE)
#define ROW_SIZE  (ID_SIZE+USERNAME_SIZE+EMAIL_SIZE)

/* Forward declaration to avoid circular dependency */
struct Table;

typedef struct{
    struct Table* table; 
    uint32_t page_num; 
    uint32_t cell_num; 
    bool end_of_table;

}Cursor;
typedef struct{
    uint32_t id; 
    char username[COLUMN_USERNAME_SIZE+1];
    char email[COLUMN_EMAIL_SIZE+1];
}Row;

/* Forward declaration */
typedef struct Table Table;

Cursor* table_start(Table* table);
Cursor* table_find(Table* table, uint32_t key);
Cursor* leaf_node_find(Table* table, uint32_t page_num, uint32_t key);
void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);
void* cursor_value(Cursor* cursor);
void cursor_advance(Cursor* cursor);
void print_row(Row* row);

void* get_page(void* pager, uint32_t page_num);