#ifndef __LIST_MY__
#define __LIST_MY__

#include "dequeue.h"

typedef struct _node {
    void* data;
    struct _node* next;
    struct _node* prev;
} Node;

struct _dequeue {
    Node* head;
    Node* tail;
};

Node* get_head(const Dequeue* dequeue);
Node* get_tail(const Dequeue* dequeue);
Node* get_node(void* data);
void set_head(Dequeue* dequeue, Node* head);
void set_tail(Dequeue* dequeue, Node* tail);
int init_dequeue(Dequeue** dequeue, int _);

Node* get_next(const Node* node);
Node* get_prev(const Node* node);
void set_next(Node* node, Node* next);
void set_prev(Node* node, Node* prev);
void* get_data(const Node* node);
void set_data(Node* node, void* data);

void dealloc_node(Node* node);

#endif