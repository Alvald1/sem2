#include "info.h"

#include <stddef.h>
#include <stdlib.h>

#include "info_lib.h"

Foo
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->key_print == NULL || info->data_print == NULL
        || info->key_dealloc == NULL || info->data_dealloc == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
info_init(Info** info, fptr_compare compare, fptr_default key_print, fptr_default data_print, fptr_default key_dealloc,
          fptr_default data_dealloc) {
    if (info == NULL) {
        return BAD_DATA;
    }
    *info = (Info*)malloc(sizeof(Info));
    if (*info == NULL) {
        return BAD_ALLOC;
    }
    (*info)->compare = compare;
    (*info)->key_print = key_print;
    (*info)->data_print = data_print;
    (*info)->key_dealloc = key_dealloc;
    (*info)->data_dealloc = data_dealloc;
    return OK;
}

void
info_dealloc(Info* info) {
    free(info);
}