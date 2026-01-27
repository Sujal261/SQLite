#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include"../include/utlis.h"
#include"../include/tree.h"
#include"../include/table.h"

void print_prompt(){
    printf("db> ");
}

void read_input(InputBuffer* input_buffer){
    
    ssize_t bytereads = getline(&(input_buffer->buffer),&(input_buffer->buffer_length), stdin);
    if(bytereads<=0){
        printf("error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytereads-1; 
    input_buffer->buffer[bytereads-1]=0;
}
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}
InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL; 
    input_buffer->buffer_length = 0; 
    input_buffer->input_length =0; 
}
void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level){
    void* node = get_page(pager, page_num);
    uint32_t num_keys, child;
    switch(get_node_type(node)){
        case NODE_LEAF:
            num_keys = *leaf_node_num_cells(node);
            indent(indentation_level);
            printf("- leaf(size %d)\n", num_keys);
            for(uint32_t i=0;i<num_keys;i++){
                indent(indentation_level+1);
                printf("- %d\n",*leaf_node_key(node, i));
            }
        break;
        case NODE_INTERNAL:
            num_keys = *internal_node_num_keys(node);
            indent(indentation_level);
            printf("- internal(size %d)\n",num_keys);
            for(uint32_t i=0;i<num_keys;i++){
                child = *internal_node_child(node,i);
                print_tree(pager, child, indentation_level+1);
                indent(indentation_level+1);
                printf("- key %d\n", *internal_node_key(node, i));

            }
            child = *internal_node_right_child(node);
            print_tree(pager, child, indentation_level+1);
            break;
    
    }

   
}

void print_constants() {
    printf("PAGE_SIZE: %u\n", PAGE_SIZE);
    printf("TABLE_MAX_PAGES: %u\n", TABLE_MAX_PAGES);

    printf("\n=== Common Node Header ===\n");
    printf("NODE_TYPE_SIZE: %lu\n", NODE_TYPE_SIZE);
    printf("NODE_TYPE_OFFSET: %u\n", NODE_TYPE_OFFSET);
    printf("IS_ROOT_SIZE: %lu\n", IS_ROOT_SIZE);
    printf("IS_ROOT_OFFSET: %lu\n", IS_ROOT_OFFSET);
    printf("PARENT_POINTER_SIZE: %lu\n", PARENT_POINTER_SIZE);
    printf("PARENT_POINTER_OFFSET: %lu\n", PARENT_POINTER_OFFSET);
    printf("COMMON_NODE_HEADER_SIZE: %lu\n", COMMON_NODE_HEADER_SIZE);

    printf("\n=== Leaf Node Header ===\n");
    printf("LEAF_NODE_NUM_CELL_SIZE: %lu\n", LEAF_NODE_NUM_CELL_SIZE);
    printf("LEAF_NODE_NUM_CELLS_OFFSET: %lu\n", LEAF_NODE_NUM_CELLS_OFFSET);
    printf("LEAF_NODE_HEADER_SIZE: %lu\n", LEAF_NODE_HEADER_SIZE);

    printf("\n=== Leaf Node Body ===\n");
    printf("LEAF_NODE_KEY_SIZE: %lu\n", LEAF_NODE_KEY_SIZE);
    printf("LEAF_NODE_KEY_OFFSET: %u\n", LEAF_NODE_KEY_OFFSET);
    printf("LEAF_NODE_VALUE_SIZE: %lu\n", LEAF_NODE_VALUE_SIZE);
    printf("LEAF_NODE_VALUE_OFFSET: %lu\n", LEAF_NODE_VALUE_OFFSET);
    printf("LEAF_NODE_CELL_SIZE: %lu\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %lu\n",LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %lu\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %lu\n", LEAF_NODE_MAX_CELLS);

    printf("\n=== Leaf Node Split ===\n");
    printf("LEAF_NODE_RIGHT_SPLIT: %lu\n", LEAF_NODE_RIGHT_SPLIT);
    printf("LEAF_NODE_LEFT_SPLIT: %lu\n", LEAF_NODE_LEFT_SPLIT);

    printf("\n=== Internal Node Header ===\n");
    printf("INTERNAL_NODE_NUM_KEYS_SIZE: %lu\n", INTERNAL_NODE_NUM_KEYS_SIZE);
    printf("INTERNAL_NODE_NUM_KEYS_OFFSET: %lu\n", INTERNAL_NODE_NUM_KEYS_OFFSET);
    printf("INTERNAL_NODE_RIGHT_CHILD_SIZE: %lu\n", INTERNAL_NODE_RIGHT_CHILD_SIZE);
    printf("INTERNAL_NODE_RIGHT_CHILD_OFFSET: %lu\n", INTERNAL_NODE_RIGHT_CHILD_OFFSET);
    printf("INTERNAL_NODE_HEADER_SIZE: %lu\n", INTERNAL_NODE_HEADER_SIZE);

    printf("\n=== Internal Node Body ===\n");
    printf("INTERNAL_NODE_KEY_SIZE: %lu\n", INTERNAL_NODE_KEY_SIZE);
    printf("INTERNAL_NODE_CHILD_SIZE: %lu\n", INTERNAL_NODE_CHILD_SIZE);
    printf("INTERNAL_NODE_CELL_SIZE: %lu\n", INTERNAL_NODE_CELL_SIZE);
}

// #define PAGE_SIZE  4096
// #define TABLE_MAX_PAGES  100
// #define LEAF_NODE_RIGHT_SPLIT (LEAF_NODE_MAX_CELLS+1)/2
// #define LEAF_NODE_LEFT_SPLIT (LEAF_NODE_MAX_CELLS+1)-LEAF_NODE_RIGHT_SPLIT
// //Here we are defining the header for the internal node 

// #define INTERNAL_NODE_NUM_KEYS_SIZE sizeof(uint32_t)
// #define INTERNAL_NODE_NUM_KEYS_OFFSET COMMON_NODE_HEADER_SIZE
// #define INTERNAL_NODE_RIGHT_CHILD_SIZE sizeof(uint32_t)
// #define INTERNAL_NODE_RIGHT_CHILD_OFFSET (INTERNAL_NODE_NUM_KEYS_OFFSET+INTERNAL_NODE_NUM_KEYS_SIZE)
// #define INTERNAL_NODE_HEADER_SIZE (COMMON_NODE_HEADER_SIZE+INTERNAL_NODE_NUM_KEYS_SIZE+INTERNAL_NODE_RIGHT_CHILD_SIZE)

// //here we are definin the body of internal node

// #define INTERNAL_NODE_KEY_SIZE sizeof(uint32_t)
// #define INTERNAL_NODE_CHILD_SIZE sizeof(uint32_t)
// #define INTERNAL_NODE_CELL_SIZE (INTERNAL_NODE_KEY_SIZE+INTERNAL_NODE_CHILD_SIZE)



// #define NODE_TYPE_SIZE sizeof(uint8_t)
// #define NODE_TYPE_OFFSET 0
// #define IS_ROOT_SIZE sizeof(uint8_t)
// #define IS_ROOT_OFFEST NODE_TYPE_SIZE 
// #define PARENT_POINTER_SIZE sizeof(uint32_t)
// #define PARENT_POINTER_OFFSET IS_ROOT_OFFEST+IS_ROOT_SIZE
// #define COMMON_NODE_HEADER_SIZE NODE_TYPE_SIZE+IS_ROOT_SIZE+PARENT_POINTER_SIZE

// //Leaf Node header Format

// #define LEAF_NODE_NUM_CELL_SIZE sizeof(uint32_t)
// #define LEAF_NODE_NUM_CELLS_OFFSET COMMON_NODE_HEADER_SIZE 
// #define LEAF_NODE_HEADER_SIZE COMMON_NODE_HEADER_SIZE +LEAF_NODE_NUM_CELL_SIZE

// //Leaf Node body format
// #define LEAF_NODE_KEY_SIZE sizeof(uint32_t)
// #define LEAF_NODE_KEY_OFFSET 0 
// #define LEAF_NODE_VALUE_OFFSET LEAF_NODE_KEY_OFFSET+LEAF_NODE_KEY_SIZE
// #define LEAF_NODE_VALUE_SIZE ROW_SIZE 
// #define LEAF_NODE_CELL_SIZE (LEAF_NODE_KEY_SIZE+LEAF_NODE_VALUE_SIZE)
// #define LEAF_NODE_SPACE_FOR_CELLS (PAGE_SIZE - LEAF_NODE_HEADER_SIZE)
// #define LEAF_NODE_MAX_CELLS (LEAF_NODE_SPACE_FOR_CELLS/LEAF_NODE_CELL_SIZE)



void indent(uint32_t indentation_level){
    for(uint32_t i=0; i<indentation_level;i++){
        printf(" ");
    }
}