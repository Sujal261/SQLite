#pragma once
#include<stdint.h>
#include<string.h>
#include"type.h"




#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

#define ID_SIZE  size_of_attribute(Row, id)
#define USERNAME_SIZE  size_of_attribute(Row, username)
#define EMAIL_SIZE  size_of_attribute(Row ,email)
#define ID_OFFSET  0
#define USERNAME_OFFSET  (ID_OFFSET+ID_SIZE)
#define EMAIL_OFFSET  (USERNAME_OFFSET+USERNAME_SIZE)
#define ROW_SIZE  (ID_SIZE+USERNAME_SIZE+EMAIL_SIZE)
struct Table; 
void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);
void* row_slot(struct Table* table, uint32_t row_num);