#ifndef __PATIENT__
#define __PATIENT__

#include <stdio.h>

typedef struct _patient {
    char* id;
    size_t ta;
    size_t ts;
    char status;
} Patient;

void set_id(Patient* patient, char* id);
void set_ta(Patient* patient, size_t ta);
void set_ts(Patient* patient, size_t ts);
void set_status(Patient* patient, char status);

char* get_id(const Patient* patient);
size_t get_ta(const Patient* patient);
size_t get_ts(const Patient* patient);
char get_status(const Patient* patient);

void print_patient(void* patient);

void fill_patient(Patient* patient, char* id, size_t get_ta, size_t get_ts, char get_status);

void dealloc_patient(Patient* patient);

#endif