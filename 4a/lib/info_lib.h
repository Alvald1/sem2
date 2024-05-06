#ifndef LAB4A_4A_LIB_INFO_LIB_H_
#define LAB4A_4A_LIB_INFO_LIB_H_

#include "code_status.h"
#include "info.h"

struct _info {
    fptr_compare compare;
};

Foo __info_valid(Info* info);

#endif