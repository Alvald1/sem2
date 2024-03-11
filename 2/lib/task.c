#include "task.h"
#include <errno.h>
#include <readline/readline.h>
#include <stdio.h>
#include <string.h>
#include "code_status.h"
#include "patient.h"

int read_time(char* str, size_t* time);
int task(Dequeue* dequeue);
Patient* read_patient(char* info);
int valid_id(char* str);

int
task(Dequeue* dequeue) {
    (void)dequeue;
    char* line = NULL;
    char* save_line = NULL;
    char* info = NULL;
    while ((line = readline("Введите строку\n"))) {
        while ((info = strtok_r(line, " ", &save_line))) {
            printf("%s\n", info);
        }
    }
    return OK;
}

Patient*
read_patient(char* info) {
    char* save_info = NULL;
    char* tmp = NULL;
    char* id = NULL;
    size_t ta = 0, ts = 0;
    char status = 0;
    int stage = 1, flag = 1;
    while (flag && (tmp = strtok_r(info, "/", &save_info))) {
        switch (stage) {
            case 1:
                if (valid_id(tmp)) {
                    id = strdup(tmp);
                    status = id[0] == '*';
                    ++stage;
                } else {
                    stage = flag = 0;
                }
                break;
            case 2:
                if (read_time(tmp, &ta)) {
                    ++stage;
                } else {
                    stage = flag = 0;
                }
                break;
            case 3:
                if (read_time(tmp, &ts)) {
                    flag = 0;
                } else {
                    stage = flag = 0;
                }
                break;
        }
    }
    if (flag == stage) {
        free(id);
        return NULL;
    }
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
    size_t value = strtol(str, end_ptr, 10);
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