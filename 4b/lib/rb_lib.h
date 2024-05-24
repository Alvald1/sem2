#ifndef LAB4A_4A_LIB_TREE_LIB_H_
#define LAB4A_4A_LIB_TREE_LIB_H_

#include "code_status.h"
#include "info.h"
#include "rb.h"

typedef enum _color { RED, BLACK } Color;

struct _list {
    List* next;
    void* data;
    size_t release;
};

struct _node {
    void* key;
    List* list;
    Color color;
    Node *left, *right, *parent;
};

struct _rb {
    Node* root;
    Info* info;
};

typedef void (*fptr_action)(Node* node, RB* rb);

Foo __rb_dealloc(RB* rb);
void __rb_2D(RB* rb, Node* node, size_t space);
void __rb_desc(RB* rb, Node* node);
Foo __rb_postorder(RB* rb, fptr_action action);
void __rb_transplant(RB* rb, Node* u, Node* v);
Foo __rb_valid(RB* rb);
Node* __node_minimum(Node* root);
Foo __node_init(Node** node, void* key, void* data);
Foo __list_push(List** list, void* data);
void __rb_insert_fixup(RB* rb, Node* node);

#endif