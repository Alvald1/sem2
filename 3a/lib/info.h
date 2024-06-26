#ifndef LAB3A_3A_LIB_INFO_H_
#define LAB3A_3A_LIB_INFO_H_

typedef int (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void* data);
#include <stddef.h>
#include "code_status.h"

typedef struct _info {
    fptr_compare compare;
    fptr_default key_print, data_print, key_dealloc, data_dealloc;
    size_t key_size, data_size;
} Info;

Foo __info_valid(Info* info);

#endif