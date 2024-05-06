#ifndef LAB4A_4A_LIB_TREE_H_
#define LAB4A_4A_LIB_TREE_H_

#include "code_status.h"
#include "info.h"

typedef struct _tree Tree;

Foo tree_init(Tree** tree, Info* info);
Foo tree_insert(Tree* tree, void* key, void* data, void* result);
Foo tree_delete(Tree* tree, void* key);
Foo tree_postorder(Tree* tree);
Foo tree_search(Tree* tree, void* key, Tree** result);
Foo tree_maximum(Tree* tree);
Foo tree_dealloc(Tree* tree);

#endif