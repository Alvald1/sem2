#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"

Patient*
get_patient(Dequeue* dequeue, int pos) {
    Patient** patients = get_patients(dequeue);
    return patients[pos];
}

Patient*
pop_front(Dequeue* dequeue) {
    int head = get_head(dequeue);
    set_head(dequeue, head + 1);
    return get_patient(dequeue, head);
}

void
dealloc_dequeue(Dequeue* dequeue) {
    free(dequeue->patients);
}

void
print_dequeue(const Dequeue* dequeue) {
    int len = get_len(dequeue);
    int head = get_head(dequeue);
    int tail = get_tail(dequeue);
    Patient** patients = get_patients(dequeue);
    for (int i = head; i < len; ++i) {
        print_patient(patients[i]);
    }
    for (int i = 0; i <= tail; ++i) {
        print_patient(patients[i]);
    }
    printf("\n");
}

void
set_patients(Dequeue* dequeue, Patient** patients) {
    dequeue->patients = patients;
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

Patient**
get_patients(const Dequeue* dequeue) {
    return dequeue->patients;
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

int
init_dequeu(Dequeue* dequeue, int len) {
    Patient** patients = (Patient**)malloc(len * sizeof(Patient*));
    if (patients == NULL) {
        return BAD_ALLOC;
    }
    set_patients(dequeue, patients);
    set_len(dequeue, len);
    set_head(dequeue, len);
    set_tail(dequeue, -1);
    return OK;
}

int
push_front(Dequeue* dequeue, Patient* patient) {
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    Patient** patients = get_patients(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    if (head == 0) {
        head = len - 1;
    } else {
        --head;
    }
    patients[head] = patient;
    ++cnt;
    set_cnt(dequeue, cnt);
    set_head(dequeue, head);
    return OK;
}

int
push_back(Dequeue* dequeue, Patient* patient) {
    int tail = get_tail(dequeue);
    int len = get_len(dequeue);
    int cnt = get_cnt(dequeue);
    Patient** patients = get_patients(dequeue);
    if (cnt == len) {
        return OVERFLOW;
    }
    tail = (tail + 1) % len;
    patients[tail] = patient;
    ++cnt;
    set_cnt(dequeue, cnt);
    set_tail(dequeue, tail);
    return OK;
}