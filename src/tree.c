#include<stdint.h>
#include<stdio.h>
#include"../include/tree.h"

uint32_t* leaf_node_num_cells(void* node){
    return (uint32_t*)((char*)node + LEAF_NODE_NUM_CELLS_OFFSET);
}
void* leaf_node_cell(void* node, uint32_t cell_num){
    return (char*)node+LEAF_NODE_HEADER_SIZE+cell_num*LEAF_NODE_CELL_SIZE;
}
uint32_t* leaf_node_key(void* node, uint32_t cell_num){
    return (uint32_t*)leaf_node_cell(node, cell_num);
}
void* leaf_node_value(void* node, uint32_t cell_num){
    return (char*)leaf_node_cell(node, cell_num)+LEAF_NODE_KEY_SIZE;

}
void initialize_leaf_node(void* node){
    set_node_type(node, NODE_LEAF);
    set_root_node(node, false); 
    *leaf_node_num_cells(node)=0;
    *leaf_node_next_leaf(node)=0;
};

void set_node_type(void* node, NodeType type){
    uint8_t value = type;
    *((uint8_t*)((char*)node+NODE_TYPE_OFFSET)) = value;
}
void initialize_internal_node(void* node){
    set_node_type(node, NODE_INTERNAL);
    set_root_node(node, false);
    *internal_node_num_keys(node)=0;
    *internal_node_right_child(node) = INVALID_PAGE_NUM;
}
uint32_t* leaf_node_next_leaf(void* node){
    return (uint32_t*)((char*)node+LEAF_NODE_NEXT_LEAF_OFFSET);
}