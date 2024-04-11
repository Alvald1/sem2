#ifndef __MAIN__
#define __MAIN__

#include <stddef.h>
#include "lib/table.h"

void dealloc(void* data);
void print_item(void* data);
int cmp(void* left, void* right);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);

#endif