#ifndef __PATIENT__
#define __PATIENT__

typedef struct _patient {
    char* id;
    int ta;
    int ts;
    char status;
} Patient;

void init_patient(Patient* patient);

void set_id(Patient* patient, char* id);
void set_ta(Patient* patient, int ta);
void set_ts(Patient* patient, int ts);
void set_status(Patient* patient, char status);

char* get_id(const Patient* patient);
int get_ta(const Patient* patient);
int get_ts(const Patient* patient);
char get_status(const Patient* patient);

void print_patient(void* patient);

void fill_patient(Patient* patient, char* id, int get_ta, int get_ts, char get_status);

void dealloc_patient(Patient* patient);

#endif