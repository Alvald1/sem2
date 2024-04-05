#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"

typedef int (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void* data);

typedef struct _info {
    fptr_compare compare;
    fptr_default key_print, data_print, key_dealloc, data_dealloc;
} Info;

typedef struct _item {
    void* key;
    void* data;
} Item;

typedef struct _table {
    Item** items;
    size_t size;
    size_t cnt;
    Info* info;
} Table;

void __item_dealloc(fptr_default key_dealloc, fptr_default data_dealloc, Item* item);

void nothing(void* data);

void print(void* data);

int __info_valid(Info* info);

int __table_valid(Table* table);

int table_init(Table* table, size_t size, Info* info);

size_t __table_cnt(Table* table);

size_t __table_size(Table* table);

int __item_valid(Item* item);

int table_search(Table* table, void* key, Item** result);

int __table_search(Table* table, void* key, size_t* result);

int cmp(void* left, void* right);

int __item_make(Item** item, void* key, void* data);

int table_print(Table* table);

void __set_cnt(Table* table, size_t cnt);
int table_insert(Table* table, void* key, void* data);

int __table_insert(Table* table, Item* item);

void table_dealloc(Table* table);

void* __table_data(Table* table, size_t pos);

int item_print(Info* info, Item* item);

void
nothing(void* data) {
    (void)data;
}

void
print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

int
__info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->data_print == NULL || info->key_print == NULL) {
        return BAD_DATA;
    }
    return OK;
}

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

int
__item_valid(Item* item) {
    if (item == NULL || item->key == NULL || item->data == NULL) {
        return BAD_DATA;
    }
    return OK;
}

void*
__table_data(Table* table, size_t pos) {
    return (table->items)[pos]->data;
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
    if (__item_make(result, key, __table_data(table, pos_res)) == BAD_ALLOC) {
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
cmp(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return 1;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return 0;
    } else if (*((size_t*)left) < *((size_t*)right)) {
        return -1;
    }
    return OK;
}

int
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

int
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
    if ((cnt = __table_cnt(table)) == __table_size(table)) {
        return OVERFLOW;
    }
    Item** items = table->items;
    size_t pos_del = 0;
    switch (__table_search(table, key, &pos_del)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return BAD_KEY;
    }
    __item_dealloc(table->info->key_dealloc, table->info->data_dealloc, items[pos_del]);
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
    size_t cnt = table->cnt;
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
            __item_dealloc(table->info->key_dealloc, table->info->data_dealloc, items[result]);
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

void
table_dealloc(Table* table) {
    size_t cnt = __table_cnt(table);
    Item** items = table->items;
    fptr_default key_dealloc = table->info->key_dealloc;
    fptr_default data_dealloc = table->info->data_dealloc;
    for (size_t i = 0; i < cnt; ++i) {
        __item_dealloc(key_dealloc, data_dealloc, items[i]);
    }
    free(items);
}

void
__item_dealloc(fptr_default key_dealloc, fptr_default data_dealloc, Item* item) {
    (*key_dealloc)(item->key);
    (*data_dealloc)(item->data);
    free(item);
}

int
main() {
    Table table;
    Info info;
    info.compare = cmp;
    info.data_print = print;
    info.key_print = print;
    info.data_dealloc = nothing;
    info.key_dealloc = nothing;
    size_t k1 = 1, k2 = 3, k3 = 5, k4 = 0, k5 = 5, k6 = 11;
    size_t data = 1, data2 = 0;
    table_init(&table, 10, &info);
    table_insert(&table, &k1, &data);
    table_insert(&table, &k2, &data);
    table_insert(&table, &k3, &data);
    table_insert(&table, &k6, &data);
    table_insert(&table, &k5, &data2);
    table_insert(&table, &k4, &data);
    table_print(&table);
    table_remove(&table, &k1);
    table_print(&table);

    table_dealloc(&table);
    return 0;
}