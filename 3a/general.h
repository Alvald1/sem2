#ifndef LAB3A_3A_GENERAL_H_
#define LAB3A_3A_GENERAL_H_

#include <stddef.h>

void dealloc(void* data);
void print_item(void* data);
int cmp(void* left, void* right);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);

#endif