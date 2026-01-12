#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../include/table.h"
#include"../include/row.h"

Cursor* table_start(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table; 
    cursor->row_num = 0; 
    cursor->end_of_table = (table->num_rows == 0);

    return cursor;
}

Cursor* table_end(Table* table){
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table; 
    cursor->row_num = table->num_rows; 
    cursor->end_of_table = true;
    return cursor;
}

void serialize_row(Row* source, void* destination){
    memcpy(destination+ID_OFFSET, &(source->id), ID_SIZE);
    memcpy(destination+USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
    memcpy(destination+EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void* source, Row* destination){
    memcpy(&(destination->id), source+ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source+USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source+EMAIL_OFFSET, EMAIL_SIZE);
}
// void* row_slot(struct Table* table, uint32_t row_num){
//     uint32_t page_num = row_num/ROWS_PER_PAGE;
//     void* page = get_page(table->pager, page_num);
//     uint32_t row_offset = row_num % ROWS_PER_PAGE; 
//     uint32_t byte_offset = row_offset * ROW_SIZE; 
//     return page+byte_offset;
// }

void* cursor_value(Cursor* cursor){
    uint32_t row_num = cursor->row_num;
    uint32_t page_num = row_num/ROWS_PER_PAGE; 
    void* page = get_page(cursor->table->pager, page_num);
    uint32_t row_offset = row_num % ROWS_PER_PAGE; 
    uint32_t byte_offset = row_num * ROW_SIZE; 
    return page+byte_offset;
}
void cursor_advance(Cursor* cursor){
    cursor->row_num+=1; 
    if(cursor->row_num >= cursor->table->num_rows){
        cursor->end_of_table = true; 
    }
}

void print_row(Row* row){
    printf("(%d,%s,%s)\n", row->id, row->username,row->email );
}
void* get_page(Pager* pager, uint32_t page_num){
    if(page_num> TABLE_MAX_PAGES){
        printf("Tried to fetch the page number out of bounds. %d > %d\n", page_num, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }
    if(pager->pages[page_num]==NULL){
        //Cache miss Allocate memory and load from file.
        void* page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length/PAGE_SIZE;

        if(pager->file_length % PAGE_SIZE){
            num_pages+=1;
        }
        if(page_num<=num_pages){
            lseek(pager->file_descriptor ,page_num*PAGE_SIZE, SEEK_SET);
            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
            if(bytes_read == -1){
                printf("Error reading file:%d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[page_num]  = page; 
    }
    return pager->pages[page_num];
}