#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
#include "dequeue.h"

struct _dequeue {
    Patient** patients;
    int len;
    int head;
    int tail;
    int cnt;
};

void set_cnt(Dequeue* dequeue, int cnt);
void set_len(Dequeue* dequeue, int len);

int get_len(const Dequeue* dequeue);
int get_cnt(const Dequeue* dequeue);

Patient* get_patient(Dequeue* dequeue, int pos);

void set_patients(Dequeue* dequeue, Patient** patients);
void set_head(Dequeue* dequeue, int head);
void set_tail(Dequeue* dequeue, int tail);

Patient** get_patients(const Dequeue* dequeue);
int get_head(const Dequeue* dequeue);
int get_tail(const Dequeue* dequeue);

Patient*
get_patient(Dequeue* dequeue, int pos) {
    Patient** patients = get_patients(dequeue);
    return patients[pos];
}

Patient*
pop_front(Dequeue* dequeue) {
    int head = get_head(dequeue);
    int len = get_len(dequeue);
    void* res = NULL;
    if (head == len) {
        head = 0;
    }
    set_head(dequeue, head + 1);
    res = get_patient(dequeue, head);
    return (Patient*)res;
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
init_dequeu(Dequeue** dequeue, int len) {
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    Patient** patients = malloc(len * sizeof(Patient*));
    if (patients == NULL) {
        return BAD_ALLOC;
    }
    set_patients(*dequeue, patients);
    set_len(*dequeue, len);
    set_head(*dequeue, len);
    set_tail(*dequeue, -1);
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