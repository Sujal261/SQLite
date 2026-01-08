#pragma once
#include<stdint.h>
#include<sys/types.h>

#define COLUMN_USERNAME_SIZE 32 
#define COLUMN_EMAIL_SIZE 255

typedef struct{
    uint32_t id; 
    char username[COLUMN_USERNAME_SIZE+1];
    char email[COLUMN_EMAIL_SIZE+1];
}Row;
typedef struct{
    char* buffer; 
    size_t buffer_length; 
    ssize_t input_length;
} InputBuffer;



typedef enum{
    PREPARE_SUCCESS,
    PREPARE_FAILURE, 
    PREPARE_SYNTAX_ERROR

} PrepareResult;

typedef enum{
    STATEMENT_INSERT, 
    STATEMENT_SELECT,
    STATEMENT_CREATE

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
    EXECUTE_TABLE_FULL
}ExecuteResult;