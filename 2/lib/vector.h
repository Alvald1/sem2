#ifndef __VECTOR_MY__
#define __VECTOR_MY__

#include "patient.h"

typedef struct _dequeue {
    Patient** patients;
    int len;
    int head;
    int tail;
    int cnt;
} Dequeue;

void set_patients(Dequeue* dequeue, Patient** patients);
void set_len(Dequeue* dequeue, int len);
void set_head(Dequeue* dequeue, int head);
void set_tail(Dequeue* dequeue, int tail);
void set_cnt(Dequeue* dequeue, int cnt);

Patient** get_patients(const Dequeue* dequeue);
int get_len(const Dequeue* dequeue);
int get_head(const Dequeue* dequeue);
int get_tail(const Dequeue* dequeue);
int get_cnt(const Dequeue* dequeue);

int init_dequeu(Dequeue* dequeue, int len);

int push_front(Dequeue* dequeue, Patient* patient);
int push_back(Dequeue* dequeue, Patient* patient);

void print_dequeue(const Dequeue* dequeue);

void dealloc_dequeue(Dequeue* dequeue);

Patient* pop_front(Dequeue* dequeue);
Patient* get_patient(Dequeue* dequeue, int pos);

#endif