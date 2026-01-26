#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<stdint.h>
#include"table.h"
#include"row.h"

typedef enum{
    NODE_INTERNAL,
    NODE_LEAF
}NodeType; 
//Node header layout
#define NODE_TYPE_SIZE sizeof(uint8_t)
#define NODE_TYPE_OFFSET 0
#define IS_ROOT_SIZE sizeof(uint8_t)
#define IS_ROOT_OFFSET NODE_TYPE_SIZE 
#define PARENT_POINTER_SIZE sizeof(uint32_t)
#define PARENT_POINTER_OFFSET IS_ROOT_OFFSET+IS_ROOT_SIZE
#define COMMON_NODE_HEADER_SIZE NODE_TYPE_SIZE+IS_ROOT_SIZE+PARENT_POINTER_SIZE

//Leaf Node header Format

#define LEAF_NODE_NUM_CELL_SIZE sizeof(uint32_t)
#define LEAF_NODE_NUM_CELLS_OFFSET COMMON_NODE_HEADER_SIZE
#define LEAF_NODE_NEXT_LEAF_SIZE sizeof(uint32_t)
#define LEAF_NODE_NEXT_LEAF_OFFSET (LEAF_NODE_NUM_CELLS_OFFSET+LEAF_NODE_NUM_CELL_SIZE)
#define LEAF_NODE_HEADER_SIZE (COMMON_NODE_HEADER_SIZE +LEAF_NODE_NUM_CELL_SIZE+LEAF_NODE_NEXT_LEAF_SIZE)


//Leaf Node body format
#define LEAF_NODE_KEY_SIZE sizeof(uint32_t)
#define LEAF_NODE_KEY_OFFSET 0 
#define LEAF_NODE_VALUE_OFFSET (LEAF_NODE_KEY_OFFSET+LEAF_NODE_KEY_SIZE)
#define LEAF_NODE_VALUE_SIZE ROW_SIZE 
#define LEAF_NODE_CELL_SIZE (LEAF_NODE_KEY_SIZE+LEAF_NODE_VALUE_SIZE)
#define LEAF_NODE_SPACE_FOR_CELLS (PAGE_SIZE-LEAF_NODE_HEADER_SIZE)
#define LEAF_NODE_MAX_CELLS (LEAF_NODE_SPACE_FOR_CELLS/LEAF_NODE_CELL_SIZE)

uint32_t* leaf_node_num_cells(void* node);
void* leaf_node_cell(void* node, uint32_t cell_num);
uint32_t* leaf_node_key(void* node, uint32_t cell_num);
void* leaf_node_value(void* node, uint32_t cell_num);
void initialize_leaf_node(void* node);
void set_node_type(void* node, NodeType type);
NodeType get_node_type(void* node);
void initialize_internal_node(void* node);
uint32_t* leaf_node_next_leaf(void* node);