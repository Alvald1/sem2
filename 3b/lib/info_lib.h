#ifndef LAB3B_3B_LIB_INFO_LIB_H_
#define LAB3B_3B_LIB_INFO_LIB_H_

#include "code_status.h"
#include "info.h"

struct _info {
    fptr_compare compare;
    fptr_default key_dealloc, data_dealloc;
    fptr_default key_print, data_print;
    size_t key_size, data_size;
};

Foo __info_valid(Info* info);

#endif