#include "info.h"

#include <stddef.h>

#include "info_lib.h"

Foo
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL) {
        return BAD_DATA;
    }
    return OK;
}