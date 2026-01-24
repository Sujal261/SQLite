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

void print_constants(){
    printf("ROW_SIZE:%ld\n", ROW_SIZE); 
    printf("COMMON_NODE_HEADER_SIZE: %ld\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %ld\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS:%ld\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_HEADER_SIZE: %ld \n",LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_MAX_CELLS: %ld\n", LEAF_NODE_MAX_CELLS);

}
void indent(uint32_t indentation_level){
    for(uint32_t i=0; i<indentation_level;i++){
        printf(" ");
    }
}