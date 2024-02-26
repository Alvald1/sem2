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

int
init_dequeue(Dequeue** dequeue, int _, size_t size) {
    (void)_;
}