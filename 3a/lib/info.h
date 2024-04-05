#ifndef __INFO__
#define __INFO__

typedef int (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void* data);
#include <stddef.h>

typedef struct _info {
    fptr_compare compare;
    fptr_default key_print, data_print, key_dealloc, data_dealloc;
    size_t key_size, data_size;
} Info;

int __info_valid(Info* info);

#endif