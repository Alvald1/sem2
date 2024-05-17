#ifndef LAB4A_4A_LIB_TREE_LIB_H_
#define LAB4A_4A_LIB_TREE_LIB_H_

#include "code_status.h"
#include "info.h"
#include "tree.h"

struct _node {
    void *key, *data;
    Node *left, *right, *parent;
};

struct _tree {
    Node* root;
    Info* info;
};

#define COUNT 10

typedef void (*fptr_action)(Node* node, Tree* tree);

Foo __tree_dealloc(Tree* tree);
void __tree_2D(Tree* tree, Node* node, size_t space);
void __tree_desc(Tree* tree, Node* node);
Foo __tree_postorder(Tree* tree, fptr_action action);
void __tree_transplant(Tree* tree, Node* u, Node* v);
Foo __tree_valid(Tree* tree);
Node* __node_minimum(Node* root);
Foo __node_init(Node** node, void* key, void* data);

#endif