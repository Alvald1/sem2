#ifndef LAB3B_3B_MAIN_H_
#define LAB3B_3B_MAIN_H_

#include <stddef.h>

#include "lib/code_status.h"
#include "lib/table.h"

size_t* gen_number(size_t value);
void dealloc(void* data);
void print(void* data);
int compare(void* left, void* right);
Foo insert(Table* table);
Foo _remove(Table* table);
Foo search(Table* table, Info* info);
Foo _export(Table* table);
Foo import(Table** table, Info* info);
void _print(Table* table);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);

#endif