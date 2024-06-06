#include "queue.h"

#include <stdlib.h>

Node*
node_new(size_t data) {
    Node* node = (Node*)malloc(sizeof(Node));
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
    Node* node = node_new(data);
    if (queue->rear == NULL) {
        queue->front = queue->rear = node;
        return;
    }
    queue->rear->next = node;
    queue->rear = node;
}

void
deQueue(Queue* queue) {
    if (queue->front == NULL) {
        return;
    }
    Node* node = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(node);
}

void
queue_dealloc(Queue* queue) {
    Node *node = queue->front, *previous = NULL;
    while (node != NULL) {
        previous = node;
        node = node->next;
        free(previous);
    }
    free(queue);
}