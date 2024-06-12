#ifndef LAB4A_4A_LIB_FREC_DICT_H_
#define LAB4A_4A_LIB_FREC_DICT_H_

#include <stddef.h>

#include "tree.h"

char* __convert_to_buty(void* data, size_t size);
size_t __convert_to_num(void* data);
Foo frec_dict(Tree* tree, const char* file_name);
void __node_print(Node* node, Tree* tree);

#endif