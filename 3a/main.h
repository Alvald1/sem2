#ifndef __MAIN__
#define __MAIN__

#include <stddef.h>

void dealloc(void* data);
void print(void* data);
int cmp(void* left, void* right);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);
void task(Table* table, size_t left, size_t right);

#endif