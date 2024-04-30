#include "hash.h"

size_t
hash(void* key, size_t size, size_t capacity) {
    char* number = (char*)key;
    size_t hash = 0;
    int a = 31415, b = 27183;
    for (; --size; ++number, a = a * b % (capacity - 1)) {
        hash = (a * hash + *number) % capacity;
    }
    return hash;
}