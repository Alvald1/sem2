#ifndef LAB4A_4A_MAIN_H_
#define LAB4A_4A_MAIN_H_

#include <stddef.h>

#include "lib/tree.h"

Compare compare(void* left, void* right);
size_t* gen_number(size_t value);
void key_print(void* key);
void data_print(void* data);
void dealloc(void* data);
Foo insert(Tree* tree);
Foo _delete(Tree* tree);
Foo search(Tree* tree);
void max(Tree* tree);
void print_desc(Tree* tree);
void print_postorder(Tree* tree);
int get_number(const char* format, void* number);
int read_num(size_t* num, const char* prompt);

#endif