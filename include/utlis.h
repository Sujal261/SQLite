#pragma once
#include"type.h"
#include"table.h"

InputBuffer* new_input_buffer();
void print_prompt();
void read_input(InputBuffer* input_buffer);
void close_input_buffer(InputBuffer* input_buffer);

// void print_leaf_node(void* node);

void print_constants();
void indent(uint32_t indentation_level);

void print_tree(Pager* pager, uint32_t page_num, uint32_t indentation_level);