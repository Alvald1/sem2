#include "task.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_status.h"
#include "patient.h"
#include "read.h"
#include "sort.h"

int
task(Dequeue* dequeue) {
    char* line = NULL;
    char* save_line = NULL;
    char* info = NULL;
    Patient* patient = NULL;
    Patients* patients = NULL;
    while ((line = readline("Введите строку\n"))) {
        if (make_patients(&patients) == BAD_ALLOC) {
            return BAD_ALLOC;
        }
        info = strtok_r(line, " ", &save_line);
        do {
            switch (read_patient(info, &patient)) {
                case BAD_READ: break;
                case BAD_ALLOC:
                    free(line);
                    dealloc_patients(patients, patients->len);
                    return BAD_ALLOC;
                case OK:
                    if (append(patients, patient) == BAD_ALLOC) {
                        free(line);
                        dealloc_patients(patients, patients->len);
                        return BAD_ALLOC;
                    }
                    break;
            }
        } while ((info = strtok_r(NULL, " ", &save_line)));
        gnome_sort(patients);
        size_t len = patients->len;
        size_t clock = 1, i = 0;
        size_t window_end = 0;
        Patient* window = NULL;
        fptr_push push = NULL;
        int flag = 1;
        while (flag) {
            while (i < len && (patients->arr)[i]->ta == clock) {
                if ((patients->arr)[i]->status) {
                    push = push_front;
                } else {
                    push = push_back;
                }
                switch ((*push)(dequeue, (void*)(patients->arr)[i])) {
                    case BAD_DEQUEUE:
                        free(line);
                        dealloc_patients(patients, len);
                        return BAD_DEQUEUE;
                    case BAD_ALLOC:
                        free(line);
                        dealloc_patients(patients, len);
                        return BAD_ALLOC;
                    case OVERFLOW: fprintf(stderr, "OVERFLOW\n"); break;
                }
                ++i;
            }
            if (window == NULL || clock == window_end) {
                if (pop_front(dequeue, (void*)&window) != EMPTY) {
                    window_end = clock + window->ts;
                }
            }
            if (window) {
                printf("%zu\n%s\t", clock, window->id);
            }
            print_dequeue(dequeue, print_patient);
            printf("\n");
            if (i < len) {
                clock = (patients->arr)[i]->ta;
            } else {
                clock = window_end;
            }
            if (i == len && is_empty(dequeue)) {
                flag = 0;
            }
        }
        free(line);
        dealloc_patients(patients, len);
    }
    return OK;
}

void
dealloc_patients(Patients* patients, size_t cnt) {
    if (patients == NULL) {
        return;
    }
    for (size_t i = 0; i < cnt; ++i) {
        dealloc_patient((patients->arr)[i]);
    }
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
