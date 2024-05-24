#ifndef LAB4A_4A_LIB_TREE_H_
#define LAB4A_4A_LIB_TREE_H_

#include "code_status.h"
#include "info.h"

typedef struct _tree Tree;
typedef struct _node Node;

Foo tree_init(Tree** tree, Info* info);
Foo tree_insert(Tree* tree, void* key, void* data, void** result);
Foo tree_delete(Tree* tree, void* key);
Foo tree_search(Tree* tree, void* key, Node** result);
Foo tree_print_postorder(Tree* tree);
void tree_print_2D(Tree* tree);
void tree_print_desc(Tree* tree);
Foo tree_dealloc(Tree* tree);
Node* tree_maximum(Tree* tree);
void node_print(Node* node, Tree* tree);
void node_dealloc(Node* node, Tree* tree);

#endif