#include "hash.h"

size_t
hash(char* number, size_t capacity) {
    size_t hash = 0;
    int a = 31415, b = 27183;
    for (; *number; ++number, a = a * b % (capacity - 1)) {
        hash = (a * hash + *number) % capacity;
    }
    return hash;
}