#ifndef __TASK__
#define __TASK__

#include "dequeue.h"
#include "patient.h"

typedef struct _patients {
    Patient** arr;
    size_t len;
} Patients;

typedef int (*fptr_push)(Dequeue* dequeue, void* data);

int task(Dequeue* dequeue);
int append(Patients* patients, Patient* patient);
int make_patients(Patients** patients);
void dealloc_patients(Patients* patients, size_t cnt);

#endif