#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_status.h"
#include "item_lib.h"
#include "table_lib.h"

int
__table_valid(Table* table) {
    if (table == NULL || table->items == NULL || __info_valid(table->info) == NOT_FOUND) {
        return BAD_DATA;
    }
    return OK;
}

int
table_init(Table* table, size_t size, Info* info) {
    if (table == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    Item** items = (Item**)malloc(size * sizeof(Item*));
    if (items == NULL) {
        return BAD_ALLOC;
    }
    table->size = size;
    table->items = items;
    table->cnt = 0;
    table->info = info;
    return OK;
}

size_t
__table_cnt(Table* table) {
    return table->cnt;
}

size_t
__table_size(Table* table) {
    return table->size;
}

void*
__table_data(Table* table, size_t pos) {
    return (table->items)[pos]->data;
}

int
__copy(size_t size, void* src, void** dest) {
    *dest = malloc(size);
    if (*dest == NULL) {
        return BAD_ALLOC;
    }
    memcpy(*dest, src, size);
    return OK;
}

int
table_search(Table* table, void* key, Item** result) {
    size_t pos_res = 0;
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    switch (__table_search(table, key, &pos_res)) {
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return NOT_FOUND;
    }
    void* new_data = NULL;
    if (__copy(table->info->data_size, __table_data(table, pos_res), &new_data) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    if (__item_make(result, key, new_data) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    return OK;
}

int
__table_search(Table* table, void* key, size_t* result) {
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    size_t middle = 0, left = 0, right = __table_cnt(table);
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

int
table_print(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t cnt = __table_cnt(table);
    if (cnt) {
        printf("\nkey\tdata\n");
    }
    Item** items = table->items;
    for (size_t i = 0; i < cnt; ++i) {
        item_print(table->info, items[i]);
    }
    return OK;
}

void
__set_cnt(Table* table, size_t cnt) {
    table->cnt = cnt;
}

int
table_remove(Table* table, void* key) {
    if (__table_valid(table) || key == NULL) {
        return BAD_DATA;
    }
    size_t cnt = table->cnt;
    Item** items = table->items;
    size_t pos_del = 0;
    switch (__table_search(table, key, &pos_del)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return BAD_KEY;
    }
    item_dealloc(table->info, items[pos_del]);
    for (size_t i = pos_del; i < cnt - 1; ++i) {
        items[i] = items[i + 1];
    }
    __set_cnt(table, cnt - 1);
    return OK;
}

int
table_insert(Table* table, void* key, void* data) {
    Item* item = NULL;
    switch (__item_make(&item, key, data)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_KEY: return BAD_KEY;
        case BAD_ALLOC: return BAD_ALLOC;
    }
    return __table_insert(table, item);
}

int
__table_insert(Table* table, Item* item) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t cnt;
    if ((cnt = __table_cnt(table)) == __table_size(table)) {
        return OVERFLOW;
    }
    size_t i;
    void* key = item->key;
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    size_t result = 0;
    switch (__table_search(table, key, &result)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case OK:
            item_dealloc(table->info, items[result]);
            items[result] = item;
            return OK;
    }
    for (i = cnt; i > 0 && (*compare)(items[i - 1]->key, key) == 1; --i) {
        items[i] = items[i - 1];
    }
    items[i] = item;
    __set_cnt(table, cnt + 1);
    return OK;
}

int
table_dealloc(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t cnt = __table_cnt(table);
    Item** items = table->items;
    for (size_t i = 0; i < cnt; ++i) {
        item_dealloc(table->info, items[i]);
    }
    free(items);
    return OK;
}