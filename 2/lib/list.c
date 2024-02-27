#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
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
Node* get_node(const void* data);
void set_head(Dequeue* dequeue, Node* head);
void set_tail(Dequeue* dequeue, Node* tail);
int init_dequeue(Dequeue** dequeue, int _);

Node* get_next(const Node* node);
Node* get_prev(const Node* node);
void set_next(Node* node, Node* next);
void set_prev(Node* node, Node* prev);
void* get_data(const Node* node);
void set_data(Node* node, void* data);

int
push_back(Dequeue* dequeue, void* data) {
    Node* new_node = get_node(data);
    if (new_node == NULL) {
        return BAD_ALLOC;
    } else {
    }
}

Node*
get_node(const void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    set_data(new_node, data);
    set_next(new_node, NULL);
    set_prev(new_node, NULL);
    return new_node;
}

int
init_dequeue(Dequeue** dequeue, int _) {
    (void)_;
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    return OK;
}

Node*
get_next(const Node* node) {
    return node->next;
}

Node*
get_prev(const Node* node) {
    return node->prev;
}

void
set_next(Node* node, Node* next) {
    node->next = next;
}

void
set_prev(Node* node, Node* prev) {
    node->prev = prev;
}

void*
get_data(const Node* node) {
    return node->data;
}

void
set_data(Node* node, void* data) {
    node->data = data;
}

Node*
get_head(const Dequeue* dequeue) {
    return dequeue->head;
}

void
set_head(Dequeue* dequeue, Node* head) {
    dequeue->head = head;
}

Node*
get_tail(const Dequeue* dequeue) {
    return dequeue->tail;
}

void
set_tail(Dequeue* dequeue, Node* tail) {
    dequeue->tail = tail;
}