#include "item.h"

#include "stddef.h"

#include "item_lib.h"

Foo
__items_valid(Item* items) {
    if (items == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
__item_fill(void* key, void* data, Item* items) {
    if (data = NULL) {
        return BAD_DATA;
    }
    items->key = key;
    items->data = data;
    items->busy = 1;
    return OK;
}