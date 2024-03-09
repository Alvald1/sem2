#ifndef __DEQUEUE__
#define __DEQUEUE__

#include <stdio.h>

typedef struct _dequeue Dequeue;
typedef void (*fptr_print_data)(void* data);

int push_front(Dequeue* dequeue, void* data);
int push_back(Dequeue* dequeue, void* data);

int pop_front(Dequeue* dequeue, void** data);
int pop_back(Dequeue* dequeue, void** data);

int print_dequeue(const Dequeue* dequeue, fptr_print_data fptr);

int dealloc_dequeue(Dequeue* dequeue);

int init_dequeue(Dequeue** dequeue, size_t len);

#endif