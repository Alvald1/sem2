#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_status.h"
#include "item_lib.h"
#include "table_lib.h"

Foo
__table_valid(Table* table) {
    if (table == NULL || table->items == NULL || __info_valid(table->info) == NOT_FOUND) {
        return BAD_DATA;
    }
    return OK;
}

Foo
table_init(Table* table, size_t capacity, Info* info) {
    if (table == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    Item** items = (Item**)malloc(capacity * sizeof(Item*));
    if (items == NULL) {
        return BAD_ALLOC;
    }
    table->capacity = capacity;
    table->items = items;
    table->size = 0;
    table->info = info;
    return OK;
}

size_t
__table_size(Table* table) {
    return table->size;
}

size_t
__table_capacity(Table* table) {
    return table->capacity;
}

void*
__table_data(Table* table, size_t pos) {
    return (table->items)[pos]->data;
}

Foo
table_search(Table* table, void* key, Item** result) {
    size_t pos_res = 0;
    if (__table_valid(table) == BAD_DATA || key == NULL || result == NULL) {
        return BAD_DATA;
    }
    switch (__table_search(table, key, &pos_res)) {
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return NOT_FOUND;
    }
    void* new_data = NULL;
    new_data = malloc(table->info->data_size);
    if (new_data == NULL) {
        return BAD_ALLOC;
    }
    memcpy(new_data, __table_data(table, pos_res), table->info->data_size);
    if (__item_make(result, key, new_data) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    return OK;
}

Foo
__table_search(Table* table, void* key, size_t* result) {
    if (result == NULL) {
        return BAD_DATA;
    }
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    size_t middle = 0, left = 0, right = __table_size(table);
    while (right > left) {
        middle = left + (right - left) / 2;
        switch ((*compare)(items[middle]->key, key)) {
            case 0: *result = middle; return OK;
            case 1: right = middle; break;
            case -1: left = middle + 1; break;
            default: return BAD_COMP;
        }
    }
    return NOT_FOUND;
}

Foo
table_print(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t size = __table_size(table);
    if (size) {
        printf("\nkey\tdata\n");
    }
    Item** items = table->items;
    for (size_t i = 0; i < size; ++i) {
        item_print(table->info, items[i]);
    }
    return OK;
}

void
__set_size(Table* table, size_t size) {
    table->size = size;
}

Foo
table_remove(Table* table, void* key) {
    if (__table_valid(table) || key == NULL) {
        return BAD_DATA;
    }
    size_t size = table->size;
    Item** items = table->items;
    size_t pos_del = 0;
    switch (__table_search(table, key, &pos_del)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return BAD_KEY;
    }
    item_dealloc(table->info, items[pos_del]);
    memmove(items + pos_del, items + pos_del + 1, (size - 1 - pos_del) * sizeof(Item**));
    __set_size(table, size - 1);
    return OK;
}

Foo
table_insert(Table* table, void* key, void* data) {
    Item* item = NULL;
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    if (__table_size(table) == __table_size(table)) {
        return OVERFLOW;
    }
    switch (__item_make(&item, key, data)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_KEY: return BAD_KEY;
        case BAD_ALLOC: return BAD_ALLOC;
    }
    return __table_insert(table, item);
}

Foo
__table_insert(Table* table, Item* item) {
    size_t size = __table_size(table);
    size_t i;
    void* key = item->key;
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    size_t result = 0;
    switch (__table_search(table, key, &result)) {
        case BAD_DATA: item_dealloc(table->info, item); return BAD_DATA;
        case BAD_COMP: item_dealloc(table->info, item); return BAD_COMP;
        case OK: item_dealloc(table->info, item); return BAD_KEY;
    }
    for (i = size; i > 0 && (*compare)(items[i - 1]->key, key) == 1; --i)
        ;
    memmove(items + i + 1, items + i, (size - i) * sizeof(Item**));
    items[i] = item;
    __set_size(table, size + 1);
    return OK;
}

Foo
table_dealloc(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t size = __table_size(table);
    Item** items = table->items;
    for (size_t i = 0; i < size; ++i) {
        item_dealloc(table->info, items[i]);
    }
    free(items);
    return OK;
}
