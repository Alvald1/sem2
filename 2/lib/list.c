#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
#include "dequeue.h"

typedef struct _node {
    void* data;
    Node* next;
} Node;

struct _dequeue {
    Node* head;
    Node* tail;
};

void
init_node(Node* node) {
    node->data = NULL;
    node->next = NULL;
}

Node*
get_head(Dequeue* dequeue) {
    return dequeue->head;
}

Node*
get_tail(Dequeue* dequeue) {
    return dequeue->tail;
}

int
init_dequeue(Dequeue** dequeue, int _, size_t size) {
    (void)_;
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    init_node(get_head(*dequeue));
    init_node(get_tail(*dequeue));
    return OK;
}