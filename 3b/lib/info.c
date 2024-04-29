#include "info.h"

#include "info_lib.h"
#include "table.h"

Foo
__info_valid(Info* info) {
    if (info == NULL) {
        return BAD_DATA;
    }
    return OK;
}