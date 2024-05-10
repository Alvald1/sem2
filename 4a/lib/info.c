#include "info.h"

#include <stddef.h>
#include <stdlib.h>

#include "info_lib.h"

Foo
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->print == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
info_init(Info** info, fptr_compare compare, fptr_default print) {
    if (info == NULL) {
        return BAD_DATA;
    }
    *info = (Info*)malloc(sizeof(Info));
    if (*info == NULL) {
        return BAD_ALLOC;
    }
    (*info)->compare = compare;
    (*info)->print = print;
    return OK;
}