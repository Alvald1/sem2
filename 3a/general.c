#include "general.h"

#include <stdio.h>
#include <stdlib.h>

void
dealloc(void* data) {
    free(data);
}

void
print_item(void* data) {
    printf("%zu\t", *((size_t*)data));
}

int
cmp(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return 1;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return 0;
    } else {
        return -1;
    }
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