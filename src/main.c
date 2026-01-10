#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include"../include/type.h"
#include"../include/utlis.h"
#include"../include/compiler.h"
#include"../include/table.h"


int main(int argc, char* argv[]){
    if(argc<2){
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }
    char* filename = argv[1];
    Table* table = db_open(filename);

    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt(); 
        read_input(input_buffer); 
        if(input_buffer->buffer[0],"."){
            switch(do_meta_action(input_buffer, table)){
                case METACOMMAND_SUCCESS:
                continue;

                case METACOMMAND_UNRECOGNIZED:
                printf("Unrecognized command\n");
                continue;
            }

        }
        Statement* statement; 
        switch(prepare_statement(input_buffer, statement)){
            case PREPARE_SUCCESS:
            break;
            case PREPARE_NEGATIVE_ID:
            printf("Negative id is not allowed.\n");
            continue;
            case PREPARE_STRING_TOO_LONG:
            printf("String is too long.\n");
            continue;
            case PREPARE_SYNTAX_ERROR:
            printf("Syntax error. Could not parse statement\n");
            continue;
            case PREPARE_FAILURE:
            printf("Unrecognized keyowrd\n");
            continue;
        }
        switch(execute_statement(statement,table )){
            case EXECUTE_SUCCESS:
            printf("Executed\n");
            break;
            case EXECUTE_TABLE_FULL:
            printf("Error: Table full.\n");
            break;
        }
    }
}
