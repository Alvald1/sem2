#include "info.h"

#include <stdlib.h>

#include "info_lib.h"
#include "table.h"

Hash_Foo
__info_valid(Hash_Info* info) {
    if (info == NULL || info->compare == NULL || info->data_dealloc == NULL || info->key_dealloc == NULL
        || info->key_print == NULL || info->data_print == NULL) {
        return HASH_BAD_DATA;
    }
    return HASH_OK;
}

Hash_Foo
info_init(Hash_Info** info, fptr_compare compare, fptr_default key_dealloc, fptr_default data_dealloc,
          fptr_default key_print, fptr_default data_print, size_t data_size) {
    if (info == NULL) {
        return HASH_BAD_DATA;
    }
    *info = (Hash_Info*)malloc(sizeof(Hash_Info));
    if (*info == NULL) {
        return HASH_BAD_ALLOC;
    }
    (*info)->compare = compare;
    (*info)->data_dealloc = data_dealloc;
    (*info)->data_print = data_print;
    (*info)->key_dealloc = key_dealloc;
    (*info)->key_print = key_print;
    (*info)->data_size = data_size;
    return HASH_OK;
}

void
info_dealloc(Hash_Info* info) {
    free(info);
}