#pragma once
#include<stdint.h>
#include<sys/types.h>
#include"row.h"




typedef struct{
    char* buffer; 
    size_t buffer_length; 
    ssize_t input_length;
} InputBuffer;



typedef enum{
    PREPARE_SUCCESS,
    PREPARE_FAILURE, 
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID,
    PREPARE_SYNTAX_ERROR

} PrepareResult;

typedef enum{
    STATEMENT_INSERT, 
    STATEMENT_SELECT,
    STATEMENT_CREATE,
    STATEMENT_UPDATE

} StatementType;

typedef enum{
    METACOMMAND_SUCCESS,
    METACOMMAND_UNRECOGNIZED
} MetaCommandResult;

typedef struct{
    StatementType type;
    Row row_to_insert;
} Statement;

typedef enum{
    EXECUTE_SUCCESS, 
    EXECUTE_TABLE_FULL,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_NO_KEY_TO_UPDATE
}ExecuteResult;