#ifndef __VECTOR__
#define __VECTOR__

#include "dequeue.h"

struct _dequeue {
    void** dataset;
    size_t len;
    size_t head;
    size_t tail;
    size_t cnt;
};

void set_cnt(Dequeue* dequeue, size_t cnt);
void set_len(Dequeue* dequeue, size_t len);

size_t get_len(const Dequeue* dequeue);
size_t get_cnt(const Dequeue* dequeue);

void* get_data(const Dequeue* dequeue, size_t pos);

void set_dataset(Dequeue* dequeue, void** dataset);
void set_head(Dequeue* dequeue, size_t head);
void set_tail(Dequeue* dequeue, size_t tail);

void** get_dataset(const Dequeue* dequeue);
size_t get_head(const Dequeue* dequeue);
size_t get_tail(const Dequeue* dequeue);

#endif