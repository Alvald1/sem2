#ifndef __DEQUEUE__
#define __DEQUEUE__

typedef struct _dequeue Dequeue;
typedef void (*fptr_print_data)(void* data);

int push_front(Dequeue* dequeue, void* data);
int push_back(Dequeue* dequeue, void* data);

void* pop_front(Dequeue* dequeue);

void print_dequeue(const Dequeue* dequeue, fptr_print_data fptr);

void dealloc_dequeue(Dequeue* dequeue);

int init_dequeue(Dequeue** dequeue, int len, size_t size);

#endif