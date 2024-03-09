#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
#include "vector.h"

void
dealloc_dequeue(Dequeue* dequeue) {
    free(dequeue->dataset);
    free(dequeue);
}

void
print_dequeue(const Dequeue* dequeue, fptr_print_data fptr) {
    int len = get_len(dequeue);
    int head = get_head(dequeue);
    int tail = get_tail(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt) {
        if (head <= tail) {
            for (int i = head; i <= tail; ++i) {
                (fptr)(dataset[i]);
            }
        } else {
            for (int i = head; i < len; ++i) {
                (*fptr)(dataset[i]);
            }
            for (int i = 0; i <= tail; ++i) {
                (*fptr)(dataset[i]);
            }
        }
        printf("\n");
    }
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
    set_head(*dequeue, len);
    set_tail(*dequeue, -1);
    set_cnt(*dequeue, 0);
    return OK;
}

void*
pop_front(Dequeue* dequeue) {
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return NULL;
    }
    set_head(dequeue, (head + 1) % len);
    set_cnt(dequeue, cnt - 1);
    return get_data(dequeue, head);
}

void*
pop_back(Dequeue* dequeue) {
    int tail = get_tail(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return NULL;
    }
    if (tail == -1) {
        tail = len - 1;
    }
    set_tail(dequeue, tail - 1);
    set_cnt(dequeue, cnt - 1);
    return get_data(dequeue, tail);
}

int
push_front(Dequeue* dequeue, void* data) {
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    if (head == 0) {
        head = len - 1;
    } else {
        --head;
    }
    dataset[head] = data;
    ++cnt;
    set_cnt(dequeue, cnt);
    set_head(dequeue, head);
    return OK;
}

int
push_back(Dequeue* dequeue, void* data) {
    int tail = get_tail(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    tail = (tail + 1) % len;
    dataset[tail] = data;
    ++cnt;
    set_cnt(dequeue, cnt);
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