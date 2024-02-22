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

void set_patients(Dequeue* dequeue, Patient** patients);
void set_len(Dequeue* dequeue, size_t len);
void set_head(Dequeue* dequeue, size_t head);
void set_tail(Dequeue* dequeue, size_t tail);
void set_cnt(Dequeue* dequeue, size_t cnt);

Patient** get_patients(const Dequeue* dequeue);
size_t get_len(const Dequeue* dequeue);
size_t get_head(const Dequeue* dequeue);
size_t get_tail(const Dequeue* dequeue);
size_t get_cnt(const Dequeue* dequeue);

void init_patient(Patient* patient);
int init_dequeu(Dequeue* dequeue, size_t len);

void set_id(Patient* patient, char* id);
void set_ta(Patient* patient, size_t ta);
void set_ts(Patient* patient, size_t ts);
void set_status(Patient* patient, char status);

char* get_id(const Patient* patient);
size_t get_ta(const Patient* patient);
size_t get_ts(const Patient* patient);
char get_status(const Patient* patient);

int push_front(Dequeue* dequeue, Patient* patient);
int push_back(Dequeue* dequeue, Patient* patient);

void print_dequeue(const Dequeue* dequeue);
void print_patient(const Patient* patient);

void fill_patient(Patient* patient, char* id, size_t get_ta, size_t get_ts, char get_status);

void dealloc_dequeue(Dequeue* dequeue);
void dealloc_patient(Patient* patient);

int
main() {
    Patient p1, p2, p3;
    Dequeue dequeue;
    fill_patient(&p1, "a", 0, 0, 0);
    fill_patient(&p2, "b", 0, 0, 0);
    fill_patient(&p3, "c", 0, 0, 0);
    push_back(&dequeue, &p1);
    print_dequeue(&dequeue);
    push_front(&dequeue, &p2);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p3);
    print_dequeue(&dequeue);
    dealloc_dequeue(&dequeue);
    return 0;
}

void
dealloc_patient(Patient* patient) {
    free(patient->id);
}

void
dealloc_dequeue(Dequeue* dequeue) {
    free(dequeue->patients);
}

void
fill_patient(Patient* patient, char* id, size_t ta, size_t ts, char status) {
    char* id_src = get_id(patient);
    if (id_src != NULL) {
        free(id_src);
    }
    set_id(patient, id);
    set_ta(patient, ta);
    set_ts(patient, ts);
    set_status(patient, status);
}

void
print_patient(const Patient* patient) {
    printf("%s", patient->id);
}

void
print_dequeue(const Dequeue* dequeue) {
    size_t len = get_len(dequeue);
    size_t head = get_head(dequeue);
    size_t tail = get_tail(dequeue);
    Patient** patients = get_patients(dequeue);
    for (size_t i = head; i < len; ++i) {
        print_patient(patients[i]);
        printf("\n");
    }
    for (size_t i = tail; i <= len; --i) {
        print_patient(patients[i]);
        printf("\n");
    }
}

void
set_patients(Dequeue* dequeue, Patient** patients) {
    dequeue->patients = patients;
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
    set_patients(dequeue, &patient);
    set_len(dequeue, len);
    set_head(dequeue, 0);
    set_tail(dequeue, 0);
    return OK;
}

int
push_front(Dequeue* dequeue, Patient* patient) {
    size_t head = get_head(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
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
    size_t tail = get_tail(dequeue);
    size_t len = get_len(dequeue);
    size_t cnt = get_cnt(dequeue);
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
