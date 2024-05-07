#ifndef LAB4A_4A_LIB_TREE_H_
#define LAB4A_4A_LIB_TREE_H_

#include "code_status.h"
#include "info.h"

typedef struct _tree Tree;

Foo tree_init(Tree** root, Info* info);
Foo tree_insert(Tree* root, void* key, void* data, void** result);
Foo tree_delete(Tree* root, void* key, Tree** result);
Foo tree_postorder(Tree* root);
Foo tree_search(Tree* root, void* key, Tree** result);
Tree* tree_maximum(Tree* root);
Foo tree_dealloc(Tree* root);

#endif