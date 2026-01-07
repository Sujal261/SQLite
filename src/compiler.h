#pragma once
#include"type.h"
#include"utlis.h"
#include<stdlib.h>
#include<string.h>
#include"table.h"

MetaCommandResult do_meta_action(InputBuffer* input_buffer);
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);

ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement* statement, Table* table);
ExecuteResult execute_statement(Statement* statement, Table* table);