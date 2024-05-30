#ifndef LAB3B_3B_LIB_INFO_LIB_H_
#define LAB3B_3B_LIB_INFO_LIB_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"

struct _hash_info {
    fptr_compare compare;
    fptr_default key_dealloc, data_dealloc;
    fptr_default key_print, data_print;
    size_t data_size;
};

Hash_Foo __info_valid(Hash_Info* info);

#endif