#include "table.h"

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "info_lib.h"
#include "item_lib.h"
#include "prime.h"
#include "table_lib.h"

void
__table_fill(Table* table, size_t capacity, size_t size, Hash_Info* info, Item* items) {
    table->items = items;
    table->capacity = capacity;
    table->info = info;
    table->size = size;
    table->step = capacity / 2 + 1;
}

Hash_Foo
table_init(Table** table, size_t capacity, Hash_Info* info) {
    if (table == NULL || __info_valid(info) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    *table = (Table*)malloc(sizeof(Table));
    if (*table == NULL) {
        return HASH_BAD_ALLOC;
    }
    Item* items = (Item*)calloc(capacity, sizeof(Item));
    if (items == NULL) {
        free(*table);
        return HASH_BAD_ALLOC;
    }
    __table_fill(*table, capacity, 0, info, items);
    return HASH_OK;
}

Hash_Foo
__table_valid(Table* table) {
    if (table == NULL || __info_valid(table->info) == HASH_BAD_DATA || __item_valid(table->items) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    return HASH_OK;
}

Hash_Foo
table_insert(Table* table, void* key, void* data) {
    if (__table_valid(table) == HASH_BAD_DATA || key == NULL) {
        return HASH_BAD_DATA;
    }
    size_t ind = 0;
    size_t size = table->size, capacity = table->capacity, step = table->step;
    size_t hash_num = hash(key, capacity);
    Item* items = table->items;
    Hash_Foo call_back = HASH_OK;
    if (__table_search(table, key, &ind) == HASH_OK) {
        return HASH_BAD_KEY;
    }
    do {
        hash_num = (hash_num + step) % capacity;
        if (items[hash_num].status != HASH_BUSY) {
            break;
        } else {
            ++ind;
        }
    } while (ind <= size);
    if (__item_fill(key, data, items + hash_num) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    if (++(table->size) == capacity) {
        if ((call_back = __table_expand(table)) != HASH_OK) {
            return call_back;
        }
    }
    return HASH_OK;
}

Hash_Foo
table_remove(Table* table, void* key) {
    if (__table_valid(table) == HASH_BAD_DATA || key == NULL) {
        return HASH_BAD_DATA;
    }
    size_t pos = 0;
    if (__table_search(table, key, &pos) == HASH_OK) {
        __item_dealloc(table->items + pos, table->info);
        (table->items)[pos].status = HASH_DELETED;
        --(table->size);
        return HASH_OK;
    }
    return HASH_NOT_FOUND;
}

Hash_Foo
table_search(Table* table, void* key, Item** result) {
    if (__table_valid(table) == HASH_BAD_DATA || key == NULL) {
        return HASH_BAD_DATA;
    }
    size_t pos = 0;
    Hash_Foo call_back = HASH_OK;
    if ((call_back = __table_search(table, key, &pos)) != HASH_OK) {
        return call_back;
    }
    *result = (Item*)malloc(sizeof(Item));
    if (*result == NULL) {
        return HASH_BAD_ALLOC;
    }
    return __item_copy(*result, table->items + pos, table->info);
}

Hash_Foo
__table_search(Table* table, void* key, size_t* pos) {
    size_t ind = 0;
    size_t size = table->size, capacity = table->capacity, step = table->step;
    size_t hash_num = hash(key, capacity);
    Item* items = table->items;
    fptr_compare compare = table->info->compare;
    char flag = 0;
    if (size == 0) {
        return HASH_EMPTY;
    }
    do {
        hash_num = (hash_num + step) % capacity;
        if (items[hash_num].status == HASH_BUSY && compare((items[hash_num]).key, key) == 0) {
            flag = 1;
            break;
        } else {
            ++ind;
        }
    } while (ind <= size && items[hash_num].status != HASH_NIL);
    if (flag) {
        *pos = hash_num;
        return HASH_OK;
    }
    return HASH_NOT_FOUND;
}

Hash_Foo
table_print(Table* table) {
    if (__table_valid(table) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    size_t capacity = table->capacity;
    Item* items = table->items;
    printf("ind\tstatus\tkey\tdata\n");
    for (size_t i = 0; i < capacity; ++i) {
        printf("%zu\t%s\t", i, hash_statuses[items[i].status]);
        item_print(items + i, table->info);
    }
    return HASH_OK;
}

Hash_Foo
table_dealloc(Table* table) {
    if (__table_valid(table) == HASH_BAD_DATA) {
        return HASH_BAD_DATA;
    }
    Item* items = table->items;
    Hash_Info* info = table->info;
    size_t capacity = table->capacity;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            __item_dealloc(items + i, info);
        }
    }
    free(items);
    free(table);
    return HASH_OK;
}

Hash_Foo
__table_expand(Table* table) {
    Hash_Foo call_back = HASH_OK;
    Table* tmp = NULL;
    size_t capacity = table->capacity;
    if ((call_back = table_init(&tmp, next_prime((capacity * 2)), table->info)) != HASH_OK) {
        return call_back;
    }
    Item* items = table->items;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            table_insert(tmp, items[i].key, items[i].data);
        }
    }
    __table_fill(table, tmp->capacity, tmp->size, tmp->info, tmp->items);
    free(items);
    free(tmp);
    return HASH_OK;
}
