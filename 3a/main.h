#ifndef LAB3A_3A_MAIN_H_
#define LAB3A_3A_MAIN_H_

#include <stddef.h>

#include "lib/info.h"
#include "lib/table.h"

void dealloc(void* data);
void print_item(void* data);
int cmp(void* left, void* right);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);
Foo insert(Table* table);
Foo rem(Table* table);
Foo search(Table* table, Info* info);
void print(Table* table);
Foo file(Table** table, Info* info);
Foo task(Table* table);

#endif