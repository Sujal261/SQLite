#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include"../include/table.h"
#include"../include/row.h"
#include"../include/tree.h"
void db_close(Table* table){
    Pager* pager = table->pager; 
   
    for(uint32_t i =0; i<pager->num_pages;i++){
        if(pager->pages[i]==NULL){
            continue;
        }
        pager_flush(pager, 1);
        free(pager->pages[i]);
        pager->pages[i]=NULL;
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
    table->root_page_num =0;
    if(pager->num_pages == 0){
        void* root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
    }
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
    pager->num_pages = (file_length/PAGE_SIZE);
    if(file_length % PAGE_SIZE !=0){
        printf("Db file is not a whole number of pages.Corrupt file.\n");
        exit(EXIT_FAILURE);
    }
    for(uint32_t i =0; i<TABLE_MAX_PAGES;i++){
        pager->pages[i] = NULL;

    }
    return pager;
}

void pager_flush(Pager* pager, uint32_t page_num){
    if(pager->pages[page_num] == NULL){
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }
    off_t offset = lseek(pager->file_descriptor, page_num*PAGE_SIZE, SEEK_SET);
    if(offset == -1){
        printf("Error seeking: %d \n", errno);
        exit(EXIT_FAILURE);
    }
    ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num] ,PAGE_SIZE);

    if(bytes_written == -1){
        printf("Error writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value){
    void* node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if(num_cells >= LEAF_NODE_MAX_CELLS){
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }
    if(cursor->cell_num < num_cells){
        for(uint32_t i = num_cells; i> cursor->cell_num; i--){
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i-1), LEAF_NODE_CELL_SIZE);

        }
    }
    *(leaf_node_num_cells(node)) +=1; 
    *(leaf_node_key(node, cursor->cell_num))= key; 
    serialize_row(value, leaf_node_value(node,cursor->cell_num ));
}