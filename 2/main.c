#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"

typedef struct _patient {
    char* id;
    int ta;
    int ts;
    char status;
} Patient;

typedef struct _dequeue {
    Patient** patients;
    int len;
    int head;
    int tail;
    int cnt;
} Dequeue;

void set_patients(Dequeue* dequeue, Patient** patients);
void set_len(Dequeue* dequeue, int len);
void set_head(Dequeue* dequeue, int head);
void set_tail(Dequeue* dequeue, int tail);
void set_cnt(Dequeue* dequeue, int cnt);

Patient** get_patients(const Dequeue* dequeue);
int get_len(const Dequeue* dequeue);
int get_head(const Dequeue* dequeue);
int get_tail(const Dequeue* dequeue);
int get_cnt(const Dequeue* dequeue);

void init_patient(Patient* patient);
int init_dequeu(Dequeue* dequeue, int len);

void set_id(Patient* patient, char* id);
void set_ta(Patient* patient, int ta);
void set_ts(Patient* patient, int ts);
void set_status(Patient* patient, char status);

char* get_id(const Patient* patient);
int get_ta(const Patient* patient);
int get_ts(const Patient* patient);
char get_status(const Patient* patient);

int push_front(Dequeue* dequeue, Patient* patient);
int push_back(Dequeue* dequeue, Patient* patient);

void print_dequeue(const Dequeue* dequeue);
void print_patient(const Patient* patient);

void fill_patient(Patient* patient, char* id, int get_ta, int get_ts, char get_status);

void dealloc_dequeue(Dequeue* dequeue);
void dealloc_patient(Patient* patient);

Patient* pop_front(Dequeue* dequeue);
Patient* get_patient(Dequeue* dequeue, int pos);

int
main() {
    Patient p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;
    Dequeue dequeue;
    init_patient(&p1);
    init_patient(&p2);
    init_patient(&p3);
    init_patient(&p4);
    init_patient(&p5);
    init_patient(&p6);
    init_patient(&p7);
    init_patient(&p8);
    init_patient(&p9);
    init_patient(&p10);
    init_patient(&p11);
    init_patient(&p12);
    init_dequeu(&dequeue, 15);
    fill_patient(&p1, "a", 0, 0, 0);
    fill_patient(&p2, "b", 0, 0, 0);
    fill_patient(&p3, "c", 0, 0, 0);
    fill_patient(&p4, "d", 0, 0, 0);
    fill_patient(&p5, "e", 0, 0, 0);
    fill_patient(&p6, "f", 0, 0, 0);
    fill_patient(&p7, "g", 0, 0, 0);
    fill_patient(&p8, "h", 0, 0, 0);
    fill_patient(&p9, "j", 0, 0, 0);
    fill_patient(&p10, "k", 0, 0, 0);
    fill_patient(&p11, "l", 0, 0, 0);
    fill_patient(&p12, "m", 0, 0, 0);
    push_back(&dequeue, &p1);
    print_dequeue(&dequeue);
    push_front(&dequeue, &p2);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p3);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p4);
    print_dequeue(&dequeue);
    push_front(&dequeue, &p5);
    print_dequeue(&dequeue);
    pop_front(&dequeue);
    print_dequeue(&dequeue);
    pop_front(&dequeue);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p6);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p7);
    print_dequeue(&dequeue);
    push_front(&dequeue, &p8);
    print_dequeue(&dequeue);
    pop_front(&dequeue);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p9);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p10);
    print_dequeue(&dequeue);
    push_front(&dequeue, &p11);
    print_dequeue(&dequeue);
    push_back(&dequeue, &p12);
    print_dequeue(&dequeue);
    dealloc_dequeue(&dequeue);
    return 0;
}

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
dealloc_patient(Patient* patient) {
    free(patient->id);
}

void
dealloc_dequeue(Dequeue* dequeue) {
    free(dequeue->patients);
}

void
fill_patient(Patient* patient, char* id, int ta, int ts, char status) {
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
    printf("%s ", patient->id);
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

void
set_id(Patient* patient, char* id) {
    patient->id = id;
}

void
set_ta(Patient* patient, int ta) {
    patient->ta = ta;
}

void
set_ts(Patient* patient, int ts) {
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

int
get_ta(const Patient* patient) {
    return patient->ta;
}

int
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
