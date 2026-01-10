#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include"../include/table.h"
#include"../include/row.h"
void db_close(Table* table){
    Pager* pager = table->pager; 
    uint32_t num_full_pages = table->num_rows/ROWS_PER_PAGE;
    for(uint32_t i =0; i<num_full_pages;i++){
        if(pager->pages[i]==NULL){
            continue;
        }
        pager_flush(pager, 1, PAGE_SIZE);
        free(pager->pages[i]);
        pager->pages[i]=NULL;
    }

    uint32_t num_additional_rows = table->num_rows%ROWS_PER_PAGE; 
    if(num_additional_rows>0){
        uint32_t page_num = num_full_pages;
        if(pager->pages[page_num]!=NULL){
            pager_flush(pager, page_num, num_additional_rows* ROW_SIZE);
            free(pager->pages[page_num]);
            pager->pages[page_num] = NULL;
        }
    }
    int result = close(pager->file_descriptor);
    if(result ==-1){
        printf("Error closing db file:\n");
        exit(EXIT_FAILURE);
    }
    for(uint32_t i=0; i<TABLE_MAX_PAGES;i++){
        void* page = pager->pages[i];
        if(page){
            free(page);
            pager->pages[i]= NULL; 
               }
    }
    free(pager);
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
    off_t file_length = lseek(fd, 0, SEEK_END);//this moves the file 0 bytes from the end.
    //on success file_length has new offset 
    Pager* pager = malloc(sizeof(Pager));
    pager->file_descriptor = fd; 
    pager->file_length = file_length; 
    for(uint32_t i =0; i<TABLE_MAX_PAGES;i++){
        pager->pages[i] = NULL;

    }
    return pager;
}

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size){
    if(pager->pages[page_num] == NULL){
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }
    off_t offset = lseek(pager->file_descriptor, page_num*PAGE_SIZE, SEEK_SET);
    if(offset == -1){
        printf("Error seeking: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num] ,size);

    if(bytes_written == -1){
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}