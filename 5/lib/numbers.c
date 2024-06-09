#include "numbers.h"

#include <stdio.h>
#include <stdlib.h>

int*
gen_number(int value) {
    int* number = malloc(sizeof(int));
    if (number == NULL) {
        return NULL;
    }
    *number = value;
    return number;
}

int
__get_number(FILE* file, const char* format, void* number) {
    int call_back = fscanf(file, format, number);
    while (!call_back) {
        fscanf(file, "%*[^\n]");
        fscanf(file, "%*c");
        printf("Incorrect input\n");
        call_back = fscanf(file, format, number);
    }
    fscanf(file, "%*[^\n]");
    fscanf(file, "%*c");
    return call_back;
}

int
read_num(FILE* file, int* num, const char* prompt) {
    printf("%s", prompt);
    return __get_number(file, "%d", num);
}