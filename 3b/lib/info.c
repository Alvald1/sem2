#include "info.h"

#include "info_lib.h"
#include "table.h"

Foo
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->data_dealloc == NULL || info->key_dealloc == NULL) {
        return BAD_DATA;
    }
    return OK;
}