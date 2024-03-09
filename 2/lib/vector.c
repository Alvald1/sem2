#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
#include "vector.h"

int
dealloc_dequeue(Dequeue* dequeue) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    free(dequeue->dataset);
    free(dequeue);
    return OK;
}

int
print_dequeue(const Dequeue* dequeue, fptr_print_data fptr) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    int len = get_len(dequeue);
    int head = get_head(dequeue);
    int tail = get_tail(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt) {
        for (int i = head % len; i <= tail; i = (i + 1) % len) {
            (fptr)(dataset[i]);
        }
        printf("\n");
    }
    return OK;
}

int
init_dequeue(Dequeue** dequeue, int len) {
    if (len < 0) {
        return BAD_POS;
    }
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    void** dataset = malloc(len * sizeof(void*));
    if (dataset == NULL) {
        return BAD_ALLOC;
    }
    set_dataset(*dequeue, dataset);
    set_len(*dequeue, len);
    set_head(*dequeue, len + 1);
    set_tail(*dequeue, 0);
    set_cnt(*dequeue, 0);
    return OK;
}

int
pop_front(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return NULL;
    }
    set_head(dequeue, (head + 1) % len);
    set_cnt(dequeue, cnt - 1);
    data = get_data(dequeue, head);
    return OK;
}

int
pop_back(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    int tail = get_tail(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return NULL;
    }
    set_tail(dequeue, (tail - 1 + len) % len);
    set_cnt(dequeue, cnt - 1);
    data = get_data(dequeue, tail);
    return OK;
}

int
push_front(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    head = (head - 1 + len) % len;
    dataset[head] = data;
    set_cnt(dequeue, cnt + 1);
    set_head(dequeue, head);
    return OK;
}

int
push_back(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    int tail = get_tail(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    tail = (tail + 1) % len;
    dataset[tail] = data;
    set_cnt(dequeue, cnt + 1);
    set_tail(dequeue, tail);
    return OK;
}

void*
get_data(const Dequeue* dequeue, int pos) {
    void** dataset = get_dataset(dequeue);
    return dataset[pos];
}

void
set_dataset(Dequeue* dequeue, void** dataset) {
    dequeue->dataset = dataset;
}

void
set_len(Dequeue* dequeue, int len) {
    dequeue->len = len;
}

void
set_head(Dequeue* dequeue, int head) {
    dequeue->head = head;
}

void
set_tail(Dequeue* dequeue, int tail) {
    dequeue->tail = tail;
}

void
set_cnt(Dequeue* dequeue, int cnt) {
    dequeue->cnt = cnt;
}

void**
get_dataset(const Dequeue* dequeue) {
    return dequeue->dataset;
}

int
get_len(const Dequeue* dequeue) {
    return dequeue->len;
}

int
get_head(const Dequeue* dequeue) {
    return dequeue->head;
}

int
get_tail(const Dequeue* dequeue) {
    return dequeue->tail;
}

int
get_cnt(const Dequeue* dequeue) {
    return dequeue->cnt;
}