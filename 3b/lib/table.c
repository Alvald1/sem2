#include "table.h"

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "info_lib.h"
#include "item_lib.h"
#include "prime.h"
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
    (*table)->items = (Item*)calloc(capacity, sizeof(Item));
    if ((*table)->items == NULL) {
        free(*table);
        return BAD_ALLOC;
    }
    (*table)->capacity = capacity;
    (*table)->info = info;
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
table_insert(Table** table, void* key, void* data) {
    if (__table_valid(*table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t ind = 0;
    size_t size = (*table)->size, capacity = (*table)->capacity;
    size_t hash_num = hash(key, (*table)->info->key_size, capacity);
    Item* items = (*table)->items;
    fptr_compare compare = (*table)->info->compare;
    if (capacity == size) {
        return OVERFLOW;
    }
    do {
        hash_num = (hash_num + ind) % capacity;
        if (items[hash_num].busy == 0) {
            break;
        } else if (compare((items[hash_num]).key, key) == 0) {
            return BAD_KEY;
        } else {
            ++ind;
        }
    } while (ind <= size);
    if (__item_fill(key, data, items + hash_num) == BAD_DATA) {
        return BAD_DATA;
    }
    Foo call_back = OK;
    if (++((*table)->size) == capacity) {
        if ((call_back = __table_expand(table)) != OK) {
            return call_back;
        }
    }
    return OK;
}

Foo
table_remove(Table* table, void* key) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t pos = 0;
    if (__table_search(table, key, &pos) == OK) {
        __item_dealloc(table->items + pos, table->info);
        (table->items)[pos].busy = 0;
        --(table->size);
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
__table_search(Table* table, void* key, size_t* pos) {
    size_t ind = 0;
    size_t size = table->size, capacity = table->capacity;
    size_t hash_num = hash(key, table->info->key_size, capacity);
    Item* items = table->items;
    fptr_compare compare = table->info->compare;
    char flag = 0;
    if (size == 0) {
        return EMPTY;
    }
    do {
        hash_num = (hash_num + ind) % capacity;
        if (items[hash_num].busy && compare((items[hash_num]).key, key) == 0) {
            flag = 1;
            break;
        } else {
            ++ind;
        }
    } while (ind <= size && items[hash_num].busy);
    if (flag) {
        *pos = hash_num;
        return OK;
    }
    return NOT_FOUNDED;
}

Foo
table_print(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t capacity = table->capacity;
    Item* items = table->items;
    printf("ind\tbusy\tkey\tdata\n");
    for (size_t i = 0; i < capacity; ++i) {
        printf("%zu\t%d\t", i, items[i].busy);
        item_print(items + i, table->info);
    }
    return OK;
}

Foo
table_dealloc(Table* table) {
    if (__table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
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
    return OK;
}

Foo
__table_expand(Table** table) {
    Table* tmp = NULL;
    Foo call_back = OK;
    size_t capacity = (*table)->capacity;
    if ((call_back = table_init(&tmp, next_prime((capacity * 2)), (*table)->info)) != OK) {
        return call_back;
    }
    Item* items = (*table)->items;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            table_insert(&tmp, items[i].key, items[i].data);
        }
    }
    free(items);
    free(*table);
    *table = tmp;
    return OK;
}
