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
void rb_print_out_of_range(RB* rb, const char* left, const char* right);
Foo rb_dealloc(RB* rb);
void node_print(RB* rb, Node* node);

#endif