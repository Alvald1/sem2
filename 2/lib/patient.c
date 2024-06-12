#include "patient.h"
#include <stdio.h>
#include <stdlib.h>

void
dealloc_patient(Patient* patient) {
    if (patient == NULL) {
        return;
    }
    free(patient->id);
    free(patient);
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
print_patient(void* patient) {
    printf("%s ", ((Patient*)patient)->id);
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
