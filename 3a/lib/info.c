#include "info.h"
#include <stddef.h>

Foo
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->data_print == NULL || info->key_print == NULL) {
        return BAD_DATA;
    }
    return OK;
}
