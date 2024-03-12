#ifndef __READ__
#define __READ__

#include <stdio.h>
#include "code_status.h"
#include "task.h"

int read_time(char* str, size_t* time);
int read_patient(char* info, Patient** patient);
int valid_id(char* str);

#endif