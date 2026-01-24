#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../include/table.h"
#include"../include/row.h"
#include"../include/tree.h"
#include<sys/types.h>
#include"../include/type.h"

Cursor* table_start(Table* table){


    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table; 
    cursor->page_num = table->root_page_num; 
    cursor->cell_num =0;

    void* root_node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(root_node);
    cursor->end_of_table = (num_cells ==0);

    return cursor;
}

Cursor* table_find(Table* table, uint32_t key){
    uint32_t root_page_num = table->root_page_num;
    void* root_node = get_page(table->pager, root_page_num);
    if(get_node_type(root_node)==NODE_LEAF){
        return leaf_node_find(table, root_page_num ,key);

    }
    else{
        printf("Need to implement searching an internal node");
        exit(EXIT_FAILURE);
    }
}
Cursor* leaf_node_find(Table* table, uint32_t page_num, uint32_t key){
    void* node  = get_page(table->pager, page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    Cursor* cursor = malloc(sizeof(Cursor));
    cursor->table = table; 
    cursor->page_num = page_num; 

    uint32_t min_index = 0;
    uint32_t one_past_max_index = num_cells;
    while(one_past_max_index != min_index){
        uint32_t index = (min_index + one_past_max_index)/2;
        uint32_t key_at_index = *leaf_node_key(node, index);
        if(key == key_at_index){
            cursor->cell_num = index;
            return cursor;

        }
        if(key < key_at_index){
            one_past_max_index = index;

        }else{
            min_index = index+1;
        }
    }
    cursor->cell_num = min_index;
    return cursor;
}

NodeType get_node_type(void* node){
    uint8_t value = *((uint8_t*)(node+NODE_TYPE_OFFSET));
    return (NodeType)value;
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
    uint32_t page_num = cursor->page_num;  
    void* page = get_page(cursor->table->pager, page_num);
    return leaf_node_value(page, cursor->cell_num);
}
void cursor_advance(Cursor* cursor){
    uint32_t page_num = cursor->page_num; 
    void* node = get_page(cursor->table->pager, page_num);
    cursor->cell_num +=1; 
    if(cursor->cell_num >= (*leaf_node_num_cells(node))){
        cursor->end_of_table = true;
    }
}

void print_row(Row* row){
    printf("(%d,%s,%s)\n", row->id, row->username,row->email );
}
void* get_page(void* pager_ptr, uint32_t page_num){
    Pager* pager = (Pager*)pager_ptr;
    if(page_num>= TABLE_MAX_PAGES){
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
        if(page_num >= pager->num_pages){
            pager->num_pages = page_num+1;
        }
    }
    return pager->pages[page_num];
}
