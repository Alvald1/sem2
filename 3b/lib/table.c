#include "table.h"

#include <stdlib.h>

#include "hash.h"
#include "info_lib.h"
#include "item_lib.h"
#include "table_lib.h"

Foo
table_init(Table** table, size_t capacity, Info* info) {
    if (table == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *table = (Table*)malloc(sizeof(Table));
    if (*table == NULL) {
        return BAD_ALLOC;
    }
    Item* items = (Item*)malloc(capacity * sizeof(Item));
    if (items == NULL) {
        return BAD_ALLOC;
    }
    (*table)->capacity = capacity * 2;
    (*table)->info = info;
    (*table)->items = items;
    (*table)->size = 0;
    return OK;
}

Foo
__table_valid(Table* table) {
    if (table == NULL || __info_valid(table->info) == BAD_DATA || __items_valid(table->items) == BAD_DATA) {
        return BAD_DATA;
    }
}

Foo
table_insert(Table* table, void* key, void* data) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t j = hash(key, table->info->key_size, table->capacity);
    size_t i = 0;
    size_t size = table->size, capacity = table->capacity;
    Item* items = table->items;
    char flag = 0;
    do {
        j = (j + i) % capacity;
        if ((items[j]).busy == 0) {
            flag = 1;
            break;
        } else {
            ++i;
        }
    } while (i != size);
    if (flag == 0) {
        return OVERFLOW;
    }
    if (__item_fill(key, data, items + j) == BAD_DATA) {
        return BAD_DATA;
    }
}