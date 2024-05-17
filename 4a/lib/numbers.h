#ifndef NUMBERS_H_
#define NUMBERS_H_

#include <stddef.h>

size_t* gen_number(size_t value);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);

#endif