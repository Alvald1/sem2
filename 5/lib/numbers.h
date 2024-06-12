#ifndef NUMBERS_H_
#define NUMBERS_H_

#include <stdio.h>

int* gen_number(int value);
int __get_number(FILE* file, const char* format, void* number);
int read_num(FILE* file, int* num, const char* prompt);

#endif