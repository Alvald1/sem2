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

Foo __tree_successor(Tree* root, Tree** result);
Foo __tree_valid(Tree* root);
void __tree_fill(Tree* root, void* key, void* data);

#endif