#include "task.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_status.h"
#include "patient.h"

int read_time(char* str, size_t* time);
int task(Dequeue* dequeue);
int read_patient(char* info, Patient** patient);
int valid_id(char* str);

int
task(Dequeue* dequeue) {
    (void)dequeue;
    char* line = NULL;
    char* save_line = NULL;
    char* info = NULL;
    while ((line = readline("Введите строку\n"))) {
        info = strtok_r(line, " ", &save_line);
        do {
            printf("%s\n", info);
        } while ((info = strtok_r(NULL, " ", &save_line)));
    }
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
    tmp = strtok_r(info, "/", &save_info);
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
    if (len == 0) {
        return BAD_READ;
    }
    if (len == 1 && str[0] == '*') {
        return BAD_READ;
    }
    return OK;
}

int
read_time(char* str, size_t* time) {
    errno = 0;
    char* end_ptr = NULL;
    size_t value = strtol(str, &end_ptr, 10);
    if (errno != 0) {
        return BAD_READ;
    }
    if (str == end_ptr) {
        return BAD_READ;
    }
    if (*end_ptr != '\0') {
        return BAD_READ;
    }
    *time = value;
    return OK;
}