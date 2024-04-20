#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
table_init(Table** table, size_t capacity, Info* info) {
    if (__info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *table = (Table*)malloc(sizeof(Table));
    Item** items = (Item**)malloc(capacity * sizeof(Item*));
    if (items == NULL || table == NULL) {
        free(table);
        free(items);
        return BAD_ALLOC;
    }
    (*table)->capacity = capacity;
    (*table)->items = items;
    (*table)->size = 0;
    (*table)->info = info;
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

Item*
__table_item(Table* table, size_t pos) {
    return (table->items)[pos];
}

void*
__table_data(Table* table, size_t pos) {
    return __table_item(table, pos)->data;
}

Foo
table_remove_by_range(Table* table, void* left, void* right) {
    size_t left_ind = 0, right_ind = 0;
    switch (__table_search(table, left, &left_ind)) {
        case BAD_COMP: return BAD_COMP;
        default: break;
    }
    switch (__table_search(table, right, &right_ind)) {
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: right_ind == 0 ? 0 : --right_ind;
        default: break;
    }
    if (left_ind > right_ind) {
        return BAD_KEY;
    }
    Item** items = table->items;
    size_t size = __table_size(table);
    if (size == 0) {
        return EMPTY;
    }
    for (size_t i = left_ind; i <= right_ind; ++i) {
        item_dealloc(table->info, items[i]);
    }
    memmove(items + left_ind, items + right_ind + 1, (size - right_ind - 1) * sizeof(Item**));
    __set_size(table, size - (right_ind - left_ind + 1));
    return OK;
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
        default: break;
    }
    void* new_data = malloc(table->info->data_size);
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
    *result = right;
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
__table_remove(Table* table, void* key, size_t* pos_del) {
    if (__table_valid(table) || key == NULL) {
        return BAD_DATA;
    }
    size_t size = table->size;
    Item** items = table->items;
    switch (__table_search(table, key, pos_del)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return BAD_KEY;
        default: break;
    }
    item_dealloc(table->info, items[*pos_del]);
    memmove(items + *pos_del, items + *pos_del + 1, (size - 1 - *pos_del) * sizeof(Item**));
    __set_size(table, size - 1);
    return OK;
}

Foo
table_remove(Table* table, void* key) {
    size_t _;
    return __table_remove(table, key, _);
}

Foo
__table_insert_(Table* table, void* key, void* data, size_t* result) {
    Item* item = NULL;
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    if (__table_size(table) == __table_capacity(table)) {
        return OVERFLOW;
    }
    switch (__item_make(&item, key, data)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_KEY: return BAD_KEY;
        case BAD_ALLOC: return BAD_ALLOC;
        default: break;
    }
    return __table_insert(table, item, result);
}

Foo
table_insert(Table* table, void* key, void* data) {
    size_t _;
    return __table_insert_(table, key, data, _);
}

Foo
__table_insert(Table* table, Item* item, size_t* result) {
    size_t size = __table_size(table);
    void* key = item->key;
    Item** items = table->items;
    switch (__table_search(table, key, result)) {
        case BAD_DATA: item_dealloc(table->info, item); return BAD_DATA;
        case BAD_COMP: item_dealloc(table->info, item); return BAD_COMP;
        case OK: item_dealloc(table->info, item); return BAD_KEY;
        default: break;
    }
    memmove(items + *result + 1, items + *result, (size - *result) * sizeof(Item**));
    memcpy(items + *result, &item, sizeof(Item**));
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
    free(table);
    return OK;
}
