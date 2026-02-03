#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include"../include/table.h"
#include"../include/row.h"
#include"../include/tree.h"

void db_close(Table* table){
    Pager* pager = table->pager; 
   
    for(uint32_t i =0; i< pager->num_pages;i++){
        if(pager->pages[i]==NULL){
            continue;
        }
        pager_flush(pager, i);
        free(pager->pages[i]);
        pager->pages[i]=NULL;
    }

   
    int result = close(pager->file_descriptor);
    if(result ==-1){
        printf("Error closing db file:\n");
        exit(EXIT_FAILURE);
    }
    free(pager);
    free(table);

}
Table* db_open(const char* filename){
    Pager* pager = pager_open(filename);
    Table* table = malloc(sizeof(Table));
    table->pager = pager; 
    table->root_page_num =0;
    if(pager->num_pages == 0){
        void* root_node = get_page(pager, 0);
        initialize_leaf_node(root_node);
        set_root_node(root_node, true);
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
        leaf_node_split_and_insert(cursor, key, value);
        return;
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
void leaf_node_update(Cursor* cursor, uint32_t key, Row* value){
    void* node = get_page(cursor->table->pager, cursor->page_num);
    serialize_row(value, leaf_node_value(node, cursor->cell_num));
}


void leaf_node_split_and_insert(Cursor* cursor, uint32_t key, Row* value){
    uint32_t index_within_node =0;
    void* old_node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t old_max = get_node_max_key(cursor->table->pager,old_node);
    uint32_t new_page_num = get_unused_page_num(cursor->table->pager);
    void* new_node = get_page(cursor->table->pager, new_page_num);

    initialize_leaf_node(new_node);
    *node_parent(new_node) = *node_parent(old_node);

    *leaf_node_next_leaf(new_node) = *leaf_node_next_leaf(old_node);
    *leaf_node_next_leaf(old_node) = new_page_num;
    for(int32_t i = LEAF_NODE_MAX_CELLS;i>=0;i--){
        void* destination_node;
        if(i>=LEAF_NODE_LEFT_SPLIT){
            destination_node = new_node;
            index_within_node = i-7;
        }else{
            destination_node = old_node;
            index_within_node = i;
        }
       
        void* destination = leaf_node_cell(destination_node, index_within_node);
        if(i == cursor->cell_num){
            serialize_row(value, leaf_node_value(destination_node, index_within_node));
            *leaf_node_key(destination_node, index_within_node) = key;

        }else if(i> cursor->cell_num){
            memcpy(destination, leaf_node_cell(old_node,i-1), LEAF_NODE_CELL_SIZE);


        }else{
            memcpy(destination, leaf_node_cell(old_node, i), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leaf_node_num_cells(old_node)) = LEAF_NODE_LEFT_SPLIT;
    *(leaf_node_num_cells(new_node)) = LEAF_NODE_RIGHT_SPLIT;
    if(is_root_node(old_node)){
        return create_new_root_node(cursor->table, new_page_num);

    }else{
       uint32_t parent_page_num = *node_parent(old_node);
       uint32_t new_max = get_node_max_key(cursor->table->pager,new_node);
       void* parent = get_page(cursor->table->pager, parent_page_num);
       update_internal_node_key(parent, old_max, new_max);
       internal_node_insert(cursor->table,parent_page_num, new_page_num );
       return; 

    }

}
uint32_t* node_parent(void* node){
    return (uint32_t*)(node+PARENT_POINTER_OFFSET);
}

uint32_t get_unused_page_num(Pager* pager){
    return pager->num_pages;

}

void update_internal_node_key(void* node, uint32_t old_key, uint32_t new_key){
    uint32_t num_cells = *internal_node_num_keys(node);
    uint32_t old_child_index = internal_node_find_child(node,old_key );
    *internal_node_key(node, old_child_index) = new_key;
}
// void update_root_node_key(void* node, uint32_t old_key, uint32_t new_key){
//     uint32_t new_child_index = internal_node_find_child(node, new_key);
//     *internal_node_key(node, new_child_index) = new_key;
// }

uint32_t internal_node_find_child(void* node, uint32_t key){
    uint32_t num_keys = *internal_node_num_keys(node);
    uint32_t min_index = 0; 
    uint32_t max_index= num_keys; 
    while(min_index<max_index){
        uint32_t mid_index = (min_index+max_index)/2;
        uint32_t mid_key = *internal_node_key(node, mid_index);
        if(key<mid_key){
            max_index = mid_index;
        }else{
            min_index = max_index+1;
        }
    }
    return min_index;


}

void internal_node_insert(Table* table, uint32_t parent_page_num, uint32_t child_page_num){
    void* parent = get_page(table->pager, parent_page_num);
    void* child = get_page(table->pager, child_page_num);
    uint32_t child_max_key = get_node_max_key(table->pager, child);
    uint32_t index = internal_node_find_child(parent, child_max_key);
    uint32_t original_num_keys = *internal_node_num_keys(parent);
   
    if(original_num_keys>= INTERNAL_NODE_MAX_CELLS){
       internal_node_split_and_insert(table, parent_page_num, child_page_num);
       return;
    }
    uint32_t right_child_page_num = *internal_node_right_child(parent);
    if(right_child_page_num == INVALID_PAGE_NUM){
        *internal_node_right_child(parent) =child_page_num;
        return;
    }
    void* right_child = get_page(table->pager, right_child_page_num);
    if(child_max_key>get_node_max_key(table->pager, right_child)){
        *internal_node_child(parent, original_num_keys) = right_child_page_num;
        *internal_node_key(parent, original_num_keys) = get_node_max_key(table->pager,right_child);
        *internal_node_right_child(parent) = child_page_num;

    }else{
        for(uint32_t i = original_num_keys; i> index; i--){
            void* destination = internal_node_cell(parent, i);
            void* source = internal_node_cell(parent, i-1);
            memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);

        }
        *internal_node_child(parent, index) = child_page_num;
        *internal_node_key(parent, index) = child_max_key;
    }

}

void internal_node_split_and_insert(Table* table, uint32_t parent_page_num, uint32_t child_page_num){
    uint32_t old_page_num = parent_page_num; 
    void* old_node = get_page(table->pager, parent_page_num);
    uint32_t old_max = get_node_max_key(table->pager, old_node);
    void* child = get_page(table->pager, child_page_num);
    uint32_t child_max = get_node_max_key(table->pager, child);
    uint32_t new_page_num = get_unused_page_num(table->pager);
    uint32_t splitting_root = is_root_node(old_node);
    void* parent; 
    void* new_node; 
    if(splitting_root){
        create_new_root_node(table, new_page_num);
        parent = get_page(table->pager, table->root_page_num);
        old_page_num = *internal_node_child(parent, 0);
        old_node = get_page(table->pager, old_page_num);

    }else{
        parent = get_page(table->pager, *node_parent(old_node));
        new_node = get_page(table->pager, new_page_num);
        initialize_internal_node(new_node);
    }
    uint32_t* old_num_keys = internal_node_num_keys(old_node);
    uint32_t cur_page_num = *internal_node_right_child(old_node);
    void* cur = get_page(table->pager, cur_page_num);
    internal_node_insert(table, new_page_num, cur_page_num);
    *node_parent(cur)= new_page_num; 
    *internal_node_right_child(old_node) = INVALID_PAGE_NUM;
    for(int i = INTERNAL_NODE_MAX_CELLS-1; i>INTERNAL_NODE_MAX_CELLS/2; i--){
        cur_page_num = *internal_node_child(old_node, i);
        cur = get_page(table->pager, cur_page_num);
        internal_node_insert(table, new_page_num, cur_page_num);
        *node_parent(cur) = new_page_num;
        (*old_num_keys)--;
    }
    *internal_node_right_child(old_node) = *internal_node_child(old_node, *old_num_keys-1);
    (*old_num_keys)--;
    uint32_t max_after_split = get_node_max_key(table->pager, old_node);
    uint32_t destination_page_num = child_max < max_after_split ?old_page_num : new_page_num;
    internal_node_insert(table, destination_page_num, child_page_num);
    *node_parent(child) = destination_page_num; 
    update_internal_node_key(parent, old_max, get_node_max_key(table->pager, old_node));
    if(!splitting_root){
        internal_node_insert(table, *node_parent(old_node), new_page_num);
        *node_parent(new_node) = *node_parent(old_node);

    }

}

void create_new_root_node(Table* table, uint32_t right_child_page_num){
    void* root = get_page(table->pager, table->root_page_num);
    void* right_child = get_page(table->pager, right_child_page_num);
    uint32_t left_child_page_num = get_unused_page_num(table->pager);
    void* left_child = get_page(table->pager, left_child_page_num);
    initialize_leaf_node(left_child);
    if(get_node_type(root) ==NODE_INTERNAL){
        initialize_internal_node(right_child); 
        initialize_internal_node(left_child);
    }
    memcpy(left_child, root, PAGE_SIZE);
    // set_root_node(left_child, false);
    
    initialize_internal_node(root);
    set_root_node(root, true);
    *internal_node_num_keys(root) = 1;
    *internal_node_child(root, 0) = left_child_page_num;
    uint32_t left_child_max_key = get_node_max_key(table->pager,left_child);
    *internal_node_key(root, 0) = left_child_max_key;
    *internal_node_right_child(root) = right_child_page_num;
    *node_parent(left_child) = table->root_page_num;
    *node_parent(right_child) = table->root_page_num;

}

uint32_t* internal_node_num_keys(void* node){

    return (uint32_t*)((char*)node+INTERNAL_NODE_NUM_KEYS_OFFSET);
}

uint32_t* internal_node_child(void* node, uint32_t child_num){
    uint32_t  num_keys = *internal_node_num_keys(node);
    if(child_num>num_keys){
        printf("Tried to access child number %d > num_keys %d\n",child_num, num_keys);
        exit(EXIT_FAILURE);
    }else if(child_num==num_keys){
        // return internal_node_right_child(node);
        uint32_t* right_child = internal_node_right_child(node);
        if(*right_child == INVALID_PAGE_NUM){
            printf("Tried to access right child of node but invalid page");
            exit(EXIT_FAILURE);

        }
        return right_child;
    }else{
        // return internal_node_cell(node, child_num);
        uint32_t* child = internal_node_cell(node, child_num);
        if(*child == INVALID_PAGE_NUM){
            printf("Tried to access child %d of node , but was invalid page\n", child_num);
            exit(EXIT_FAILURE);
        }
        return child;
    }

    


}
uint32_t* internal_node_cell(void* node, uint32_t cell_num){
    return (uint32_t*)((char*)node+INTERNAL_NODE_HEADER_SIZE+cell_num* INTERNAL_NODE_CELL_SIZE);
}

uint32_t* internal_node_key(void* node, uint32_t key_num){

    return (uint32_t*)((char*)internal_node_cell(node, key_num)+INTERNAL_NODE_CHILD_SIZE);

}

uint32_t* internal_node_right_child(void* node){
    return (uint32_t*)((char*)node+INTERNAL_NODE_RIGHT_CHILD_OFFSET);

}

uint32_t get_node_max_key(Pager* pager, void* node){
   if(get_node_type(node)==NODE_LEAF){
    return *leaf_node_key(node, *leaf_node_num_cells(node)-1);
   }
   void* right_child =  get_page(pager, *internal_node_right_child(node));
   return get_node_max_key(pager, right_child);


}
bool is_root_node(void* root){
    uint8_t value= *((uint8_t*)((char*)root+IS_ROOT_OFFSET));
    return (bool)value;
}
void set_root_node(void* node, bool is_root){
    uint8_t value = is_root;
    (*(uint8_t*)((char*)node+IS_ROOT_OFFSET))=value;
}

void leaf_node_delete(Cursor* cursor, uint32_t row_to_delete){
    void* node = get_page(cursor->table->pager, cursor->page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    if(cursor->cell_num < num_cells){
        for(uint32_t i= cursor->cell_num; i< num_cells;i++){
            memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i+1), LEAF_NODE_CELL_SIZE);
        }
    }
    *(leaf_node_num_cells(node))-=1;

    if(*(leaf_node_num_cells(node))<7){
        merge_node_or_add_from_sibling(cursor);
    }else{

    free_row(leaf_node_value(node, num_cells-1));
    }
}
void merge_node_or_add_from_sibling(Cursor* cursor){
    void* left_sibling = get_page(cursor->table->pager,cursor->page_num+1);
    void* current = get_page(cursor->table->pager, cursor->page_num);
    // void* right_sibling = get_page(cursor->table->pager, cursor->page_num+2);
    uint32_t left_num_cells = *leaf_node_num_cells(left_sibling);
    // uint32_t right_num_cells = *leaf_node_num_cells(right_sibling);
    uint32_t left_max_key_old = get_node_max_key(cursor->table->pager, left_sibling); 
    uint32_t num_cells = *leaf_node_num_cells(current);

    if(left_num_cells>7){
        for(uint32_t i =num_cells; i>=1;i--){
            memcpy(leaf_node_cell(current, i), leaf_node_cell(current, i-1), LEAF_NODE_CELL_SIZE);
        }
        memcpy(leaf_node_cell(current,0), leaf_node_cell(left_sibling, left_num_cells-1), LEAF_NODE_CELL_SIZE);
        *leaf_node_num_cells(current)+=1;
        *leaf_node_num_cells(left_sibling)-=1;

        uint32_t parent_page_num = *node_parent(left_sibling);
       uint32_t new_max = get_node_max_key(cursor->table->pager,left_sibling);
       void* parent = get_page(cursor->table->pager, parent_page_num);
       update_internal_node_key(parent,left_max_key_old, new_max);
        free_row(leaf_node_value(left_sibling, left_num_cells-1));
    }
    // }else if(right_num_cells>7){
    //     memcpy(leaf_node_cell(current, num_cells+1), leaf_node_cell(right_sibling, 0), LEAF_NODE_CELL_SIZE);
    //     for(uint32_t i =0;i<right_num_cells-1;i++){
    //         memcpy(leaf_node_cell(right_sibling,i), leaf_node_cell(right_sibling,i+1), LEAF_NODE_CELL_SIZE);
    //     }
    //     *leaf_node_num_cells(current)+=1;
    //     *leaf_node_num_cells(right_sibling)-=1;
    //     free_row(right_sibling);

    // }


}


// void leaf_node_insert(Cursor* cursor, uint32_t key, Row* value){
//     void* node = get_page(cursor->table->pager, cursor->page_num);
//     uint32_t num_cells = *leaf_node_num_cells(node);

//     if(num_cells >= LEAF_NODE_MAX_CELLS){
//         leaf_node_split_and_insert(cursor, key, value);
//         return;
//     }
//     if(cursor->cell_num < num_cells){
//         for(uint32_t i = num_cells; i> cursor->cell_num; i--){
//             memcpy(leaf_node_cell(node, i), leaf_node_cell(node, i-1), LEAF_NODE_CELL_SIZE);

//         }
//     }
//     *(leaf_node_num_cells(node)) +=1; 
//     *(leaf_node_key(node, cursor->cell_num))= key; 
//     serialize_row(value, leaf_node_value(node,cursor->cell_num ));
// }