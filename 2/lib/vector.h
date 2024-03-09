#ifndef __VECTOR__
#define __VECTOR__

#include "dequeue.h"

struct _dequeue {
    void** dataset;
    int len;
    int head;
    int tail;
    int cnt;
};

void set_cnt(Dequeue* dequeue, int cnt);
void set_len(Dequeue* dequeue, int len);

int get_len(const Dequeue* dequeue);
int get_cnt(const Dequeue* dequeue);

void* get_data(const Dequeue* dequeue, int pos);

void set_dataset(Dequeue* dequeue, void** dataset);
void set_head(Dequeue* dequeue, int head);
void set_tail(Dequeue* dequeue, int tail);

void** get_dataset(const Dequeue* dequeue);
int get_head(const Dequeue* dequeue);
int get_tail(const Dequeue* dequeue);

#endif