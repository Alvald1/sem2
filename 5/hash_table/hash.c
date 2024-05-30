#include "hash.h"

#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "hash_lib.h"

size_t
hash(void* key, size_t capacity) {
    char* number = (char*)key;
    size_t size = strlen(number);
    if (number == NULL) {
        return 0;
    }
    size_t hash_num = INT_MAX;
    size_t a = 31415, b = 27183;
    for (size_t i = 0; i < size; ++i, a = a * b % (capacity - 1 + (capacity == 1))) {
        hash_num = (a * hash_num + number[i]) % capacity;
    }
    return hash_num;
}
