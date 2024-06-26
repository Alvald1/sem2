#ifndef LAB4A_4A_LIB_TREE_LIB_H_
#define LAB4A_4A_LIB_TREE_LIB_H_

#include "code_status.h"
#include "info.h"
#include "rb.h"

typedef enum _color { RED, BLACK } Color;

typedef enum _count { ONE, MANY, _NOT_FOUND } Count;

extern const char* colors[];

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
    Node* nil;
    Info* info;
};

typedef void (*fptr_action)(RB* rb, Node* node);

void __node_dealloc(RB* rb, Node* node);
Count __delete_release(RB* rb, List** list, size_t release);
Foo __rb_dealloc(RB* rb);
void __rb_2D(RB* rb, Node* node, size_t space);
void __rb_out_of_range(RB* rb, Node* node, const char* left, const char* right);
Foo __rb_postorder(RB* rb, fptr_action action);
void __rb_inorder(RB* rb, fptr_action action);
void __rb_transplant(RB* rb, Node* u, Node* v);
Foo __rb_valid(RB* rb);
void __node_print(RB* rb, Node* node, const char* left, const char* right);
Node* __node_minimum(RB* rb, Node* node);
Node* __rb_successor(RB* rb, Node* node);
Foo __node_init(RB* rb, Node** node, void* key, void* data);
Foo __list_push(List** list, void* data);
void __left_rotate(RB* rb, Node* node);
void __right_rotate(RB* rb, Node* node);
void __rb_insert_fixup(RB* rb, Node* node);
void __rb_delete_fixup(RB* rb, Node* node);

#endif