#include "patient.h"
#include <stdio.h>
#include <stdlib.h>

void
dealloc_patient(Patient* patient) {
    free(patient->id);
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
print_patient(void* patient) {
    printf("%s ", ((Patient*)patient)->id);
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