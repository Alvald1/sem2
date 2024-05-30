#include "general.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
dealloc(void* data) {
    free(data);
}

void
key_print(void* key) {
    printf("%s\t", (char*)key);
}

void
data_print(void* data) {
    printf("%zu", *((size_t*)data));
}

Compare
compare(void* left, void* right) {
    int result = strcmp((char*)left, (char*)right);
    if (result > 0) {
        return MORE;
    } else if (result == 0) {
        return EQUAL;
    } else {
        return LESS;
    }
}