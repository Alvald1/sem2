#include "table.h"

#include <stdio.h>
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
    Item* items = (Item*)calloc(capacity, sizeof(Item));
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
__table_valid(Table* table) {
    if (table == NULL || __info_valid(table->info) == BAD_DATA || __item_valid(table->items) == BAD_DATA) {
        return BAD_DATA;
    }
    return OK;
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
    fptr_compare compare = table->info->compare;
    if (table->capacity == table->size) {
        return OVERFLOW;
    }
    do {
        j = (j + i) % capacity;
        if (items[j].busy == 0) {
            break;
        } else if (compare((items[j]).key, key) == 0) {
            return BAD_KEY;
        } else {
            ++i;
        }
    } while (i <= size);
    if (__item_fill(key, data, items + j) == BAD_DATA) {
        return BAD_DATA;
    }
    ++(table->size);
    return OK;
}

Foo
table_remove(Table* table, void* key) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    Item* items = table->items;
    size_t result = 0;
    if (__table_search(table, key, &result) == OK) {
        __item_dealloc(items + result, table->info);
        items[result].busy = 0;
        return OK;
    }
    return NOT_FOUNDED;
}

Foo
table_search(Table* table, void* key, Item** result) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t pos = 0;
    Foo call_back = OK;
    if ((call_back = __table_search(table, key, &pos)) != OK) {
        return call_back;
    }
    *result = (Item*)malloc(sizeof(Item));
    if (*result == NULL) {
        return BAD_ALLOC;
    }
    return __item_copy(*result, table->items + pos, table->info);
}

Foo
__table_search(Table* table, void* key, size_t* result) {
    size_t j = hash(key, table->info->key_size, table->capacity);
    size_t i = 0;
    size_t size = table->size, capacity = table->capacity;
    Item* items = table->items;
    fptr_compare compare = table->info->compare;
    char flag = 0;
    do {
        j = (j + i) % capacity;
        if (compare((items[j]).key, key) == 0 && items[j].busy == 1) {
            flag = 1;
            break;
        } else {
            ++i;
        }
    } while (i <= size && items[j].busy == 1);
    if (flag) {
        *result = j;
        return OK;
    }
    return NOT_FOUNDED;
}

void
table_print(Table* table) {
    size_t capacity = table->capacity;
    Item* items = table->items;
    char busy = 0;
    if (table->size) {
        printf("ind\tbusy\tkey\tdata\n");
    }
    for (size_t i = 0; i < capacity; ++i) {
        printf("%zu\t%d\t", i, (busy = items[i].busy));
        item_print(items + i, table->info);
    }
}

void
table_dealloc(Table* table) {
    Item* items = table->items;
    Info* info = table->info;
    size_t capacity = table->capacity;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            __item_dealloc(items + i, info);
        }
    }
    free(items);
    free(table);
}