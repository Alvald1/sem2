#include "hash.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "hash_lib.h"

size_t
hash(void* key, size_t size, size_t capacity) {
    char* number = __convert_to_buty(key, size);
    if (number == NULL) {
        return 0;
    }
    size_t hash_num = INT_MAX;
    size_t a = 31415, b = 27183;
    for (size_t i = 0; i < size; ++i, a = a * b % (capacity - 1)) {
        hash_num = (a * hash_num + number[i]) % capacity;
    }
    free(number);
    return hash_num;
}

char*
__convert_to_buty(void* key, size_t size) {
    char* result = (char*)malloc(size);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, key, size);
    return result;
}