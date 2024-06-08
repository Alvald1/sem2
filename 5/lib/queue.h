#ifndef LAB5_5_LIB_QUEUE_LIB_H_
#define LAB5_5_LIB_QUEUE_LIB_H_

#include <stddef.h>

#include "queue.h"

typedef struct _Qnode {
    size_t data;
    struct _Qnode* next;
} QNode;

typedef struct _queue {
    QNode *front, *rear;
} Queue;

QNode* node_new(size_t data);
Queue* queue_create();
void enQueue(Queue* queue, int data);
size_t deQueue(Queue* queue);
void queue_dealloc(Queue* queue);

#endif