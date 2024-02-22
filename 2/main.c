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
    Patient* patient;
    size_t len;
    size_t head;
    size_t tail;
} Dequeue;

void set_patient(Dequeue* dequeue, const Patient* patient);
void set_len(Dequeue* dequeue, size_t len);
void set_head(Dequeue* dequeue, size_t head);
void set_tail(Dequeue* dequeue, size_t tail);

Patient* get_patient(Dequeue* dequeue);
size_t get_len(Dequeue* dequeue);
size_t get_head(Dequeue* dequeue);
size_t get_tail(Dequeue* dequeue);

void init_patient(Patient* patient);
int init_dequeu(Dequeue* dequeue, size_t len);

void set_id(Patient* patient, char* id);
void set_ta(Patient* patient, size_t ta);
void set_ts(Patient* patient, size_t ts);
void set_status(Patient* patient, char status);

char* get_id(Patient* patient);
size_t get_ta(Patient* patient);
size_t get_ts(Patient* patient);
char get_status(Patient* patient);

int
main() {

    return 0;
}

void
set_patient(Dequeue* dequeue, const Patient* patient) {
    dequeue->patient = patient;
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

Patient*
get_patient(Dequeue* dequeue) {
    return dequeue->patient;
}

size_t
get_len(Dequeue* dequeue) {
    return dequeue->len;
}

size_t
get_head(Dequeue* dequeue) {
    return dequeue->head;
}

size_t
get_tail(Dequeue* dequeue) {
    return dequeue->tail;
}

void
set_id(Patient* patient, char* id) {
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
get_id(Patient* patient) {
    return patient->id;
}

size_t
get_ta(Patient* patient) {
    return patient->ta;
}

size_t
get_ts(Patient* patient) {
    return patient->ts;
}

char
get_status(Patient* patient) {
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
}