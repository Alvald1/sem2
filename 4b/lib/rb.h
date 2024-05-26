#ifndef LAB4A_4A_LIB_TREE_H_
#define LAB4A_4A_LIB_TREE_H_

#include "code_status.h"
#include "info.h"

typedef struct _list List;
typedef struct _rb RB;
typedef struct _node Node;

Foo rb_init(RB** rb, Info* info);
Foo rb_insert(RB* rb, void* key, void* data);
Foo rb_delete(RB* rb, void* key, size_t release);
Foo rb_search(RB* rb, void* key, Node** result);
void rb_print_inorder(RB* rb);
void rb_print_postorder(RB* rb);
void rb_print_2D(RB* rb);
Foo rb_dealloc(RB* rb);
void node_print(Node* node, RB* rb);
void node_dealloc(Node* node, RB* rb);

#endif