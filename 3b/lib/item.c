#include "item.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info_lib.h"
#include "item_lib.h"

void
item_print(Item* item, Info* info) {
    if (item->status == BUSY) {
        (*info->key_print)(item->key);
        (*info->data_print)(item->data);
    }
    printf("\n");
}

Foo
__item_valid(Item* item) {
    if (item == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
__item_copy(Item* dest, Item* src, Info* info) {
    dest->key = malloc(info->key_size);
    dest->data = malloc(info->data_size);
    if (dest->key == NULL || dest->data == NULL) {
        free(dest->data);
        free(dest->key);
        free(dest);
        return BAD_ALLOC;
    }
    dest->status = src->status;
    memcpy(dest->key, src->key, info->key_size);
    memcpy(dest->data, src->data, info->data_size);
    return OK;
}

Foo
__item_fill(void* key, void* data, Item* item) {
    if (data == NULL) {
        return BAD_DATA;
    }
    item->key = key;
    item->data = data;
    item->status = BUSY;
    return OK;
}

Foo
__item_dealloc(Item* item, Info* info) {
    if (__item_valid(item) == BAD_DATA || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    info->data_dealloc(item->data);
    info->key_dealloc(item->key);
    return OK;
}

Foo
item_dealloc(Item* item, Info* info) {
    Foo call_back = OK;
    if ((call_back = __item_dealloc(item, info)) != OK) {
        return call_back;
    }
    free(item);
    return OK;
}

void*
get_key(Item* item) {
    return item->key;
}

void*
get_data(Item* item) {
    return item->data;
}