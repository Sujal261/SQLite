#pragma once
#include"type.h"
#include"table.h"

MetaCommandResult do_meta_action(InputBuffer* input_buffer, Table* table);


void print_constants();

//Prepare part
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement);
PrepareResult prepare_update(InputBuffer* input_buffer, Statement* statement);
PrepareResult prepare_delete(InputBuffer* input_buffer, Statement statement);

//Execute part
ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement* statement, Table* table);
ExecuteResult execute_statement(Statement* statement, Table* table);
ExecuteResult execute_update(Statement* statement, Table* table);
ExecuteResult execute_delete(Statement* statement,Table* table);
