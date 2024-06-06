#ifndef LAB5_5_LIB_QUEUE_LIB_H_
#define LAB5_5_LIB_QUEUE_LIB_H_

#include <stddef.h>

#include "queue.h"

typedef struct _node {
    size_t data;
    struct _node* next;
} Node;

typedef struct _queue {
    Node *front, *rear;
} Queue;

Node* node_new(size_t data);
Queue* queue_create();
void enQueue(Queue* queue, int data);
void deQueue(Queue* queue);
void queue_dealloc(Queue* queue);

#endif