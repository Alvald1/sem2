#ifndef LAB4A_4A_MAIN_H_
#define LAB4A_4A_MAIN_H_

#include <stddef.h>

#include "lib/tree.h"

Compare compare(void* left, void* right);
void key_print(void* key);
void data_print(void* data);
void dealloc(void* data);
Foo insert(Tree* tree);
Foo _delete(Tree* tree);
Foo file(Tree* tree);
Foo search(Tree* tree);
void graphviz(Tree* tree);
void max(Tree* tree);
void print_desc(Tree* tree);
void print_postorder(Tree* tree);
void print_2D(Tree* tree);

#endif