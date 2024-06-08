#include "queue.h"

#include <stdlib.h>

QNode*
node_new(size_t data) {
    QNode* node = (QNode*)malloc(sizeof(QNode));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Queue*
queue_create() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (queue == NULL) {
        return NULL;
    }
    queue->front = queue->rear = NULL;
    return queue;
}

void
enQueue(Queue* queue, int data) {
    QNode* node = node_new(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
        return;
    }
    queue->rear->next = node;
    queue->rear = node;
}

size_t
deQueue(Queue* queue) {
    if (queue->front == NULL) {
        return 0;
    }
    QNode* node = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    size_t tmp = node->data;
    free(node);
    return tmp;
}

void
queue_dealloc(Queue* queue) {
    QNode *node = queue->front, *previous = NULL;
    while (node != NULL) {
        previous = node;
        node = node->next;
        free(previous);
    }
    free(queue);
}