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

int
init_dequeue(Dequeue** dequeue, int _, size_t size) {
    (void)_;
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    init_node((*dequeue)->head);
    init_node((*dequeue)->tail);
}