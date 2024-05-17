#include "numbers.h"

#include <stdio.h>
#include <stdlib.h>

size_t*
gen_number(size_t value) {
    size_t* number = malloc(sizeof(size_t));
    if (number == NULL) {
        return NULL;
    }
    *number = value;
    return number;
}

int
get_number(const char* format, void* number) {
    int call_back = scanf(format, number);
    while (!call_back) {
        scanf("%*[^\n]");
        printf("Incorrect input\n");
        call_back = scanf(format, number);
    }
    return call_back;
}

int
read_num(size_t* num, const char* prompt) {
    printf("%s", prompt);
    return get_number("%zu", num);
}