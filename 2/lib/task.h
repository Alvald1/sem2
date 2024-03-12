#ifndef __TASK__
#define __TASK__

#include "dequeue.h"
#include "patient.h"

int read_time(char* str, size_t* time);
int task(Dequeue* dequeue);
int read_patient(char* info, Patient** patient);
int valid_id(char* str);

#endif