#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../include/type.h"
#include"../include/utlis.h"
#include"../include/row.h"
#include"../include/table.h"
#include"../include/tree.h"



MetaCommandResult do_meta_action(InputBuffer* input_buffer, Table* table){
    if((strcmp(input_buffer->buffer,"./exit"))==0){
        db_close(table);
        exit(EXIT_SUCCESS);

    }else if(strcmp(input_buffer->buffer, ".btree")==0){
        printf("Tree:\n");
        print_tree(table->pager,0,0);
        return METACOMMAND_SUCCESS;
    }else if(strcmp(input_buffer->buffer, ".constants")==0){
        printf("Constants:\n");
        print_constants();
        return METACOMMAND_SUCCESS;
    }
    
    else{
        return METACOMMAND_UNRECOGNIZED;
    }
}

PrepareResult prepare_insert(InputBuffer* input_buffer, Statement* statement){
    statement->type = STATEMENT_INSERT; 
    char* keyword = strtok(input_buffer->buffer," ");
    char* id_string = strtok(NULL," ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");
    if(id_string == NULL || username == NULL || email == NULL){
        return PREPARE_SYNTAX_ERROR;
    }
    int id = atoi(id_string);
    if(id<0){
        return PREPARE_NEGATIVE_ID;
    }
    if(strlen(username)>(COLUMN_USERNAME_SIZE+1)){
        return PREPARE_STRING_TOO_LONG;
    }
    if(strlen(email)>(COLUMN_EMAIL_SIZE+1)){
        return PREPARE_STRING_TOO_LONG;
    }
    statement->row_to_insert.id = id; 
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);
    return PREPARE_SUCCESS;
}

PrepareResult prepare_update(InputBuffer* input_buffer, Statement* statement){
    statement->type = STATEMENT_UPDATE; 
    char* keyword = strtok(input_buffer->buffer," ");
    char* id_string = strtok(NULL," ");
    char* username = strtok(NULL, " ");
    char* email = strtok(NULL, " ");
    if(id_string == NULL || username == NULL || email == NULL){
        return PREPARE_SYNTAX_ERROR;
    }
    int id = atoi(id_string);
    if(id<0){
        return PREPARE_NEGATIVE_ID;
    }
    if(strlen(username)>(COLUMN_USERNAME_SIZE+1)){
        return PREPARE_STRING_TOO_LONG;
    }
    if(strlen(email)>(COLUMN_EMAIL_SIZE+1)){
        return PREPARE_STRING_TOO_LONG;
    }
    statement->row_to_insert.id = id; 
    strcpy(statement->row_to_insert.username, username);
    strcpy(statement->row_to_insert.email, email);
    return PREPARE_SUCCESS;
}

PrepareResult prepare_delete(InputBuffer* input_buffer, Statement* statement){
    statement->type = STATEMENT_DELETE;
    char* keyword = strtok(input_buffer->buffer," ");
    char* id_string = strtok(NULL," ");
    int id = atoi(id_string);
    if(id<0){
        return PREPARE_NEGATIVE_ID;
    }
    statement->del.id =id; 
    return PREPARE_SUCCESS;
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement){
    if(strncmp(input_buffer->buffer, "insert",6)==0){
        return prepare_insert(input_buffer, statement);
    }
    if(strncmp(input_buffer->buffer, "select",6)==0){
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    if(strncmp(input_buffer->buffer,"create",6)==0){
        statement->type = STATEMENT_CREATE;
        return PREPARE_SUCCESS;
    }
    if(strncmp(input_buffer->buffer,"update",6)==0){
        statement->type = STATEMENT_UPDATE;
        return prepare_update(input_buffer,statement);
    }
    if(strncmp(input_buffer->buffer,"delete",6)==0){
        statement->type = STATEMENT_DELETE;
        return prepare_delete(input_buffer, statement);

    }
    return PREPARE_FAILURE;
}
ExecuteResult execute_insert(Statement* statement, Table* table){
    void* node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    Row* row_to_insert = &(statement->row_to_insert);
    uint32_t key_to_insert = row_to_insert->id ;
    Cursor* cursor = table_find(table, key_to_insert);
    void* node_to_insert = get_page(table->pager, cursor->page_num);
    uint32_t key_at_index = *leaf_node_key(node_to_insert, cursor->cell_num);
    if(key_at_index == key_to_insert){
        return EXECUTE_DUPLICATE_KEY;
     }
    leaf_node_insert(cursor, row_to_insert->id, row_to_insert);
    free(cursor);
    return EXECUTE_SUCCESS;

}
ExecuteResult execute_select(Statement* statement, Table* table){
    Cursor* cursor = table_start(table);

    Row row; 
    while(!(cursor->end_of_table)){
        void* node = get_page(table->pager, table->root_page_num);
        if(*leaf_node_num_cells(node)==0){
            return EXECUTE_NO_DATA;
        }
        deserialize_row(cursor_value(cursor), &row);
        print_row(&row);
        cursor_advance(cursor);
    }
    free(cursor);
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_update(Statement* statement, Table* table){
    void* node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    Row* row_to_update = &(statement->row_to_insert);
    uint32_t key_to_update = row_to_update->id;
    Cursor* cursor = table_find(table, key_to_update);
    void* node_to_update = get_page(table->pager, cursor->page_num);
    uint32_t key_at_index = *leaf_node_key(node_to_update, cursor->cell_num);
    if(key_at_index != key_to_update){
        return EXECUTE_NO_KEY;
    }
    leaf_node_update(cursor, row_to_update->id, row_to_update);
    free(cursor);
    return EXECUTE_SUCCESS;
    
}

ExecuteResult execute_delete(Statement* statement, Table* table){
    void* node = get_page(table->pager, table->root_page_num);
    uint32_t num_cells = *leaf_node_num_cells(node);
    Delete* row_to_delete = &(statement->del);
    uint32_t key_to_delete = row_to_delete->id;
    Cursor* cursor = table_find(table, key_to_delete);
    void* node_to_delete = get_page(table->pager, cursor->page_num);
    uint32_t key_at_index = *leaf_node_key(node_to_delete, cursor->cell_num);
    if(key_at_index!= key_to_delete){
        return EXECUTE_NO_KEY;
    }
    leaf_node_delete(cursor, row_to_delete->id);
    free(cursor);
    return EXECUTE_SUCCESS;

}


ExecuteResult execute_statement(Statement* statement, Table* table){
    switch(statement->type){
        case STATEMENT_INSERT:
        return execute_insert(statement, table);

        case STATEMENT_SELECT:
        return execute_select(statement, table);

        case STATEMENT_UPDATE:
        return execute_update(statement, table);

        case STATEMENT_DELETE:
        return execute_delete(statement, table);
   }
}

