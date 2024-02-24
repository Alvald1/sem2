#ifndef __VECTOR_MY__
#define __VECTOR_MY__

#include "patient.h"

typedef struct _dequeue Dequeue;

int push_front(Dequeue* dequeue, Patient* patient);
int push_back(Dequeue* dequeue, Patient* patient);

Patient* pop_front(Dequeue* dequeue);

void print_dequeue(const Dequeue* dequeue);

void dealloc_dequeue(Dequeue* dequeue);

int init_dequeu(Dequeue** dequeue, int len);
#endif