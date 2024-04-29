#include "table.h"

#include <stdlib.h>

#include "info_lib.h"
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
    (*table)->capacity = capacity;
    (*table)->info = info;
    (*table)->items = items;
    (*table)->size = 0;
    return OK;
}

Foo
__tale_valid(Table* table) {}