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
    size_t len = get_len(dequeue);
    size_t head = get_head(dequeue);
    size_t cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (cnt) {
        for (size_t i = head % len; cnt--; i = (i + 1) % len) {
            (fptr)(dataset[i]);
        }
    } else {
        printf("\n");
        return EMPTY;
    }
    printf("\n");
    return OK;
}

int
is_empty(Dequeue* dequeue) {
    return get_cnt(dequeue) == 0;
}

int
init_dequeue(Dequeue** dequeue, size_t len) {
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
pop_front(Dequeue* dequeue, void** data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    size_t head = get_head(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return EMPTY;
    }
    set_head(dequeue, (head + 1) % len);
    set_cnt(dequeue, cnt - 1);
    *data = get_data(dequeue, head % len);
    return OK;
}

int
pop_back(Dequeue* dequeue, void** data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    size_t tail = get_tail(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
    if (cnt == 0) {
        return EMPTY;
    }
    set_tail(dequeue, (tail - 1 + len) % len);
    set_cnt(dequeue, cnt - 1);
    *data = get_data(dequeue, tail % len);
    return OK;
}

int
push_front(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    size_t head = get_head(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (dataset == NULL) {
        return BAD_DEQUEUE;
    }
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
    size_t tail = get_tail(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
    void** dataset = get_dataset(dequeue);
    if (dataset == NULL) {
        return BAD_DEQUEUE;
    }
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
get_data(const Dequeue* dequeue, size_t pos) {
    void** dataset = get_dataset(dequeue);
    return dataset[pos];
}

void
set_dataset(Dequeue* dequeue, void** dataset) {
    dequeue->dataset = dataset;
}

void
set_len(Dequeue* dequeue, size_t len) {
    dequeue->len = len;
}

void
set_head(Dequeue* dequeue, size_t head) {
    dequeue->head = head;
}

void
set_tail(Dequeue* dequeue, size_t tail) {
    dequeue->tail = tail;
}

void
set_cnt(Dequeue* dequeue, size_t cnt) {
    dequeue->cnt = cnt;
}

void**
get_dataset(const Dequeue* dequeue) {
    return dequeue->dataset;
}

size_t
get_len(const Dequeue* dequeue) {
    return dequeue->len;
}

size_t
get_head(const Dequeue* dequeue) {
    return dequeue->head;
}

size_t
get_tail(const Dequeue* dequeue) {
    return dequeue->tail;
}

size_t
get_cnt(const Dequeue* dequeue) {
    return dequeue->cnt;
}