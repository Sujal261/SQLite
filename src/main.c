#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include"type.h"
#include"utlis.h"
#include"compiler.h"
#include"table.h"


int main(int argc, char* argv[]){
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    while(true){
        print_prompt(); 
        read_input(input_buffer); 
        if(input_buffer->buffer[0],"."){
            switch(do_meta_action(input_buffer)){
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
            case EXECUTE_TABLE_FULL:
            printf("Error: Table full.\n");
            break;
        }
    }
}
