#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include"../include/table.h"
#include"../include/row.h"
void free_table(Table* table){
    for(int i=0; table->pages[i];i++){
        free(table->pages[i]);
    }
    free(table);
}
Table* db_open(const char* filename){
    Pager* pager = pager_open(filename);
    uint32_t num_rows = pager->file_length/ROW_SIZE;
    Table* table = malloc(sizeof(Table));
    table->pager = pager; 
    table->num_rows = num_rows;
    return table;
}

Pager* pager_open(const char* filename){
    int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR);
    if(fd ==-1){
        printf("Unable to open file.\n");
        exit(EXIT_FAILURE);
    }
    off_t file_length = lseek(fd, 0, SEEK_END);
    Pager* pager = malloc(sizeof(Pager));
    pager->file_descriptor = fd; 
    pager->file_length = file_length; 
    for(uint32_t i =0; i<TABLE_MAX_PAGES;i++){
        pager->pages[i] = NULL;

    }
    return pager;
}