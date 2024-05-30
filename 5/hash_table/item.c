#include "item.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info_lib.h"
#include "item_lib.h"

void
item_print(Item* item, Hash_Info* info) {
    if (item->status == HASH_BUSY) {
        (*info->key_print)(item->key);
        (*info->data_print)(item->data);
    }
    printf("\n");
}

Hash_Foo
__item_valid(Item* item) {
    if (item == NULL) {
        return HASH_BAD_DATA;
    }
    return HASH_OK;
}

Hash_Foo
__item_copy(Item* dest, Item* src, Hash_Info* info) {
    dest->key = malloc(strlen((char*)src->key) + 1);
    dest->data = malloc(info->data_size);
    if (dest->key == NULL || dest->data == NULL) {
        free(dest->data);
        free(dest->key);
        free(dest);
        return HASH_BAD_ALLOC;
    }
    dest->status = src->status;
    memcpy(dest->key, src->key, strlen((char*)src->key) + 1);
    memcpy(dest->data, src->data, info->data_size);
    return HASH_OK;
}

Hash_Foo
__item_fill(void* key, void* data, Item* item) {
    if (data == NULL) {
        return HASH_BAD_DATA;
    }
    item->key = key;
    item->data = data;
    item->status = HASH_BUSY;
    return HASH_OK;
}

Hash_Foo
__item_dealloc(Item* item, Hash_Info* info) {
    if (__item_valid(item) == HASH_BAD_DATA || __info_valid(info) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    info->data_dealloc(item->data);
    info->key_dealloc(item->key);
    return HASH_OK;
}

Hash_Foo
item_dealloc(Item* item, Hash_Info* info) {
    Hash_Foo call_back = HASH_OK;
    if ((call_back = __item_dealloc(item, info)) != HASH_OK) {
        return call_back;
    }
    free(item);
    return HASH_OK;
}

void*
get_key(Item* item) {
    return item->key;
}

void*
get_data(Item* item) {
    return item->data;
}