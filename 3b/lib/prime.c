#include "prime.h"

#include "prime_lib.h"

char
__is_prime(size_t num) {
    if (num == 1) {
        return 0;
    }
    if (num <= 3) {
        return 1;
    }
    if (num % 2 == 0 || num % 3 == 0) {
        return 0;
    }
    size_t ind = 5;
    while (ind * ind <= num) {
        if (num % ind == 0 || num % (ind + 2) == 0) {
            return 0;
        }
        ind += 6;
    }
    return 1;
}

size_t
next_prime(size_t num) {
    if (num == 1) {
        return 2;
    }
    if (num & 1) {
        num += 2;
    } else {
        ++num;
    }
    while (!__is_prime(num)) {
        num += 2;
    }
    return num;
}