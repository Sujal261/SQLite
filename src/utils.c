#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include"../include/utlis.h"
#include"../include/tree.h"

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
void print_leaf_node(void* node){
    uint32_t num_cells = *leaf_node_num_cells(node); 
    printf("leaf (size %d)\n", num_cells);
    for(uint32_t i =0 ; i<num_cells;i++){
        uint32_t key = *leaf_node_key(node, i); 
        printf(" -%d: %d\n", i, key);
    }
}

void print_constants(){
    printf("ROW_SIZE:%ld\n", ROW_SIZE); 
    printf("COMMON_NODE_HEADER_SIZE: %ld\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %ld\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS:%ld\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_HEADER_SIZE: %ld \n",LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_MAX_CELLS: %ld\n", LEAF_NODE_MAX_CELLS);

}