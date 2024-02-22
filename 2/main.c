#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"

typedef struct _patient {
    char* id;
    size_t ta;
    size_t ts;
    char status;
} Patient;

typedef struct _dequeue {
    Patient** patients;
    size_t len;
    size_t head;
    size_t tail;
    size_t cnt;
} Dequeue;

void set_patients(Dequeue* dequeue, const Patient** patients);
void set_len(Dequeue* dequeue, size_t len);
void set_head(Dequeue* dequeue, size_t head);
void set_tail(Dequeue* dequeue, size_t tail);
void set_cnt(Dequeue* dequeue, size_t cnt);

Patient** get_patients(const Dequeue* dequeue);
size_t get_len(const Dequeue* dequeue);
size_t* get_head(const Dequeue* dequeue);
size_t* get_tail(const Dequeue* dequeue);
size_t* get_cnt(const Dequeue* dequeue);

void init_patient(Patient* patient);
int init_dequeu(Dequeue* dequeue, size_t len);

void set_id(Patient* patient, const char* id);
void set_ta(Patient* patient, size_t ta);
void set_ts(Patient* patient, size_t ts);
void set_status(Patient* patient, char status);

char* get_id(const Patient* patient);
size_t get_ta(const Patient* patient);
size_t get_ts(const Patient* patient);
char get_status(const Patient* patient);

int push_front(Dequeue* dequeue, Patient* patient);
int push_back(Dequeue* dequeue, Patient* patient);

int
main() {

    return 0;
}

void
set_patients(Dequeue* dequeue, const Patient** patients) {
    dequeue->patients = *patients;
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

Patient**
get_patients(const Dequeue* dequeue) {
    return dequeue->patients;
}

size_t
get_len(const Dequeue* dequeue) {
    return dequeue->len;
}

size_t*
get_head(const Dequeue* dequeue) {
    return &(dequeue->head);
}

size_t*
get_tail(const Dequeue* dequeue) {
    return &(dequeue->tail);
}

size_t*
get_cnt(const Dequeue* dequeue) {
    return &(dequeue->cnt);
}

void
set_id(Patient* patient, const char* id) {
    patient->id = id;
}

void
set_ta(Patient* patient, size_t ta) {
    patient->ta = ta;
}

void
set_ts(Patient* patient, size_t ts) {
    patient->ts = ts;
}

void
set_status(Patient* patient, char status) {
    patient->status = status;
}

char*
get_id(const Patient* patient) {
    return patient->id;
}

size_t
get_ta(const Patient* patient) {
    return patient->ta;
}

size_t
get_ts(const Patient* patient) {
    return patient->ts;
}

char
get_status(const Patient* patient) {
    return patient->status;
}

void
init_patient(Patient* patient) {
    set_id(patient, NULL);
    set_ta(patient, 0);
    set_ts(patient, 0);
    set_status(patient, 0);
}

int
init_dequeu(Dequeue* dequeue, size_t len) {
    Patient* patient = (Patient*)malloc(len * sizeof(Patient));
    if (patient == NULL) {
        return BAD_ALLOC;
    }
    init_patient(patient);
    set_patient(dequeue, &patient);
    set_len(dequeue, len);
    set_head(dequeue, 0);
    set_tail(dequeue, 0);
    return OK;
}

int
push_front(Dequeue* dequeue, Patient* patient) {
    size_t* head = get_head(dequeue);
    size_t len = get_len(dequeue);
    size_t* cnt = get_cnt(dequeue);
    Patient** patients = get_patients(dequeue);
    if (*cnt == len) {
        return OVERFLOW;
    }
    if (*head == 0) {
        *head = len - 1;
    } else {
        --head;
    }
    patients[*head] = &patient;
    ++*cnt;
    return OK;
}

int
push_back(Dequeue* dequeue, Patient* patient) {
    size_t* tail = get_tail(dequeue);
    size_t len = get_len(dequeue);
    size_t* cnt = get_cnt(dequeue);
    Patient** patients = get_patients(dequeue);
    if (*cnt == len) {
        return OVERFLOW;
    }
    *tail = (*tail + 1) % len;
    patients[*tail] = &patient;
    ++*cnt;
    return OK;
}
