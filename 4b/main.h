#ifndef LAB4A_4A_MAIN_H_
#define LAB4A_4A_MAIN_H_

#include <stddef.h>

#include "lib/rb.h"

Compare compare(void* left, void* right);
void key_print(void* key);
void data_print(void* data);
void dealloc(void* data);
Foo insert(RB* rb);
Foo _delete(RB* rb);
Foo file(RB* rb);
Foo search(RB* rb);
void graphviz(RB* rb);
void print_desc(RB* rb);
void print_postorder(RB* rb);
void print_2D(RB* rb);

#endif