#include "item.h"

#include "stddef.h"

#include "info_lib.h"
#include "item_lib.h"

Foo
__item_valid(Item* item) {
    if (item == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
__item_fill(void* key, void* data, Item* items) {
    if (data == NULL) {
        return BAD_DATA;
    }
    items->key = key;
    items->data = data;
    items->busy = 1;
    return OK;
}

Foo
item_dealloc(Item* item, Info* info) {
    if (__item_valid(item) == BAD_DATA || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    (*info).data_dealloc(item->data);
    (*info).key_dealloc(item->key);
    return OK;
}