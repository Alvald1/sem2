#include "item.h"
#include <stdio.h>
#include <stdlib.h>
#include "item_lib.h"

Foo
__item_valid(Item* item) {
    if (item == NULL || item->key == NULL || item->data == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
__item_make(Item** item, void* key, void* data) {
    if (key == NULL) {
        return BAD_KEY;
    }
    if (data == NULL) {
        return BAD_DATA;
    }
    *item = (Item*)malloc(sizeof(Item));
    if (*item == NULL) {
        return BAD_ALLOC;
    }
    (*item)->data = data;
    (*item)->key = key;
    return OK;
}

Foo
item_print(Info* info, Item* item) {
    if (__info_valid(info) == BAD_DATA || __item_valid(item) == BAD_DATA) {
        return BAD_DATA;
    }
    fptr_default key_print = info->key_print;
    fptr_default data_print = info->data_print;
    key_print(item->key);
    data_print(item->data);
    printf("\n");
    return OK;
}

Foo
item_dealloc(Info* info, Item* item) {
    if (__info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    fptr_default key_dealloc = info->key_dealloc, data_dealloc = info->data_dealloc;
    (*key_dealloc)(item->key);
    (*data_dealloc)(item->data);
    free(item);
    return OK;
}
