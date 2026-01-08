#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include"../include/utlis.h"

void print_prompt(){
    printf("db> ");
}

void read_input(InputBuffer* input_buffer){
    
    ssize_t bytereads = getline(&(input_buffer->buffer),&(input_buffer->buffer_length), stdin);
    if(bytereads<=0){
        printf("error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_length = bytereads-1; 
    input_buffer->buffer[bytereads-1]=0;
}
void close_input_buffer(InputBuffer* input_buffer){
    free(input_buffer->buffer);
    free(input_buffer);
}
InputBuffer* new_input_buffer(){
    InputBuffer* input_buffer = malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL; 
    input_buffer->buffer_length = 0; 
    input_buffer->input_length =0; 
}
