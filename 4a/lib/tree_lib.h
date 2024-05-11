#ifndef LAB4A_4A_LIB_TREE_LIB_H_
#define LAB4A_4A_LIB_TREE_LIB_H_

#include "code_status.h"
#include "info.h"
#include "tree.h"

struct _tree {
    void *key, *data;
    struct _tree *left, *right, *parent;
    Info* info;
};

typedef void (*fptr_action)(Tree* root);

void __print(Tree* root);
Foo __tree_postorder(Tree* root, fptr_action action);
void __tree_transplant(Tree** root, Tree* u, Tree* v);
Tree* __tree_minimum(Tree* root);
Foo __tree_valid(Tree* root);
void __tree_fill(Tree* root, void* key, void* data);
void __tree_dealloc(Tree* root);

#endif