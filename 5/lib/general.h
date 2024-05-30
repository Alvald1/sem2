#ifndef LAB5_5_GENERAL_H_
#define LAB5_5_GENERAL_H_

#include "hash_table/info.h"

Compare compare(void* left, void* right);
void key_print(void* key);
void data_print(void* data);
void dealloc(void* data);

#endif