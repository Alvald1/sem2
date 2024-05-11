#ifndef LAB4A_4A_LIB_INFO_LIB_H_
#define LAB4A_4A_LIB_INFO_LIB_H_

#include "code_status.h"
#include "info.h"

struct _info {
    fptr_compare compare;
    fptr_default key_print, data_print, key_dealloc, data_dealloc;
    size_t data_size;
};

Foo __info_valid(Info* info);

#endif