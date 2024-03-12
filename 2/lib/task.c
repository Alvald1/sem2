#include "task.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_status.h"
#include "patient.h"

typedef struct _patients {
    Patient** arr;
    size_t len;
} Patients;

int read_time(char* str, size_t* time);
int task(Dequeue* dequeue);
int read_patient(char* info, Patient** patient);
int valid_id(char* str);
int append(Patients* patients, Patient* patient);
int make_patients(Patients** patients);
void gnome_sort(Patients* patients);
void swap(Patient** patient_1, Patient** patient_2);
void dealloc_patients(Patients* patients);

int
task(Dequeue* dequeue) {
    (void)dequeue;
    char* line = NULL;
    char* save_line = NULL;
    char* info = NULL;
    Patient* patient = NULL;
    Patients* patients = NULL;
    if (make_patients(&patients) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    while ((line = readline("Введите строку\n"))) {
        info = strtok_r(line, " ", &save_line);
        do {
            switch (read_patient(info, &patient)) {
                case BAD_READ: break;
                case BAD_ALLOC:
                    free(line);
                    dealloc_patients(patients);
                    return BAD_ALLOC;
                case OK:
                    if (append(patients, patient) == BAD_ALLOC) {
                        free(line);
                        dealloc_patients(patients);
                        return BAD_ALLOC;
                    }
                    break;
            }
        } while ((info = strtok_r(NULL, " ", &save_line)));
        gnome_sort(patients);
    }
    return OK;
}

void
dealloc_patients(Patients* patients) {
    free(patients->arr);
    free(patients);
}

int
make_patients(Patients** patients) {
    void* tmp = (Patients*)malloc(sizeof(Patients));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    *patients = tmp;
    tmp = (Patient**)malloc(0);
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    (*patients)->arr = tmp;
    (*patients)->len = 0;
    return OK;
}

void
swap(Patient** patient_1, Patient** patient_2) {
    Patient* temp;
    temp = *patient_1;
    *patient_1 = *patient_2;
    *patient_2 = temp;
}

void
gnome_sort(Patients* patients) {
    int index = 0;
    int len = patients->len;
    Patient** arr = patients->arr;
    while (index < len) {
        if (index == 0) {
            index++;
        }
        if ((*(arr + index))->ta >= (*(arr + index - 1))->ta) {
            index++;
        } else {
            swap((arr + index), (arr + index - 1));
            index--;
        }
    }
}

int
append(Patients* patients, Patient* patient) {
    void* tmp = (Patient**)realloc(patients->arr, (patients->len + 1) * sizeof(Patient*));
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    patients->arr = tmp;
    (patients->arr)[(patients->len)++] = patient;
    return OK;
}

int
read_patient(char* info, Patient** patient) {
    char* save_info = NULL;
    char* tmp = NULL;
    char* id = NULL;
    size_t ta = 0, ts = 0;
    char status = 0;
    int stage = 1, flag = 1;
    if (!(tmp = strtok_r(info, "/", &save_info))) {
        return BAD_READ;
    }
    do {
        switch (stage) {
            case 1:
                if (valid_id(tmp) == OK) {
                    status = tmp[0] == '*';
                    if (status) {
                        id = strdup(tmp + 1);
                    } else {
                        id = strdup(tmp);
                    }
                    ++stage;
                } else {
                    stage = flag = 0;
                }
                break;
            case 2:
                if (read_time(tmp, &ta) == OK) {
                    ++stage;
                } else {
                    stage = flag = 0;
                }
                break;
            case 3:
                if (read_time(tmp, &ts) == OK) {
                    ++stage;
                    flag = 0;
                } else {
                    stage = flag = 0;
                }
                break;
        }
    } while (flag && (tmp = strtok_r(NULL, "/", &save_info)));
    if (flag == stage || stage < 4) {
        free(id);
        return BAD_READ;
    }
    *patient = (Patient*)malloc(sizeof(Patient));
    if (patient == NULL) {
        free(id);
        return BAD_ALLOC;
    }
    set_id(*patient, id);
    set_status(*patient, status);
    set_ta(*patient, ta);
    set_ts(*patient, ts);
    return OK;
}

int
valid_id(char* str) {
    if (str == NULL) {
        return BAD_READ;
    }
    size_t len = strlen(str);
    if ((len == 0) || (len == 1 && str[0] == '*')) {
        return BAD_READ;
    }
    return OK;
}

int
read_time(char* str, size_t* time) {
    errno = 0;
    char* end_ptr = NULL;
    size_t value = strtol(str, &end_ptr, 10);
    if (errno != 0 || str == end_ptr || *end_ptr != '\0') {
        return BAD_READ;
    }
    *time = value;
    return OK;
}