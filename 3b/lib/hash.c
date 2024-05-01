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
    for (size_t i = 0; i < size; ++i) {
        hash_num = (37 * hash_num + number[i]) % capacity;
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