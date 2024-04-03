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

void
nothing(void* data) {
    (void)data;
}

void
item_print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

int
info_valid(Info* info) {
    if (info == NULL || info->compare == NULL || info->data_print == NULL || info->key_print == NULL) {
        return BAD_DATA;
    }
    return OK;
}

int
table_valid(Table* table) {
    if (table == NULL || table->items == NULL || table->info == NULL) {
        return BAD_DATA;
    }
    return OK;
}

int
table_init(Table* table, size_t size, Info* info) {
    if (table == NULL || info_valid(info) == BAD_DATA) {
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
table_cnt(Table* table) {
    return table->cnt;
}

size_t
table_size(Table* table) {
    return table->size;
}

int
item_valid(Item* item) {
    if (item == NULL || item->key == NULL || item->data == NULL) {
        return BAD_DATA;
    }
    return OK;
}

int
table_search(Table* table, void* key, Item** result) {
    if (table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    size_t middle = 0, left = 0, right = table_cnt(table);
    while (right > left) {
        middle = left + (right - left) / 2;
        switch ((*compare)(items[middle]->key, key)) {
            case 0: *result = items[middle]; return OK;
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
item_make(Item** item, void* key, void* data) {
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
table_print(Table* table) {
    if (table_valid(table) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t cnt = table_cnt(table);
    if (cnt) {
        printf("key\tdata\n");
    }
    fptr_default key_print = table->info->key_print;
    fptr_default data_print = table->info->data_print;
    Item** items = table->items;
    for (size_t i = 0; i < cnt; ++i) {
        key_print(items[i]->key);
        data_print(items[i]->data);
        printf("\n");
    }
    return OK;
}

void
set_cnt(Table* table, size_t cnt) {
    table->cnt = cnt;
}

int
table_insert(Table* table, Item* item) {
    if (table_valid(table) == BAD_DATA || item_valid(item) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t cnt = table->cnt;
    if ((cnt = table_cnt(table)) == table_size(table)) {
        return OVERFLOW;
    }
    long i;
    void* key = item->key;
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    void* result = NULL;
    switch (table_search(table, key, result)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_COMP: return BAD_COMP;
        case OK: return BAD_KEY;
    }
    for (i = cnt; i > 0 && (*compare)(items[i - 1]->key, key) == 1; --i) {
        items[i] = items[i - 1];
    }
    items[i] = item;
    set_cnt(table, cnt + 1);
    return OK;
}

void
table_dealloc(Table* table) {
    size_t cnt = table_cnt(table);
    Item** items = table->items;
    fptr_default key_dealloc = table->info->key_dealloc;
    fptr_default data_dealloc = table->info->data_dealloc;
    for (size_t i = 0; i < cnt; ++i) {
        (*key_dealloc)(items[i]->key);
        (*data_dealloc)(items[i]->data);
        free(items[i]);
    }
    free(items);
}

int
main() {
    Table table;
    Info info;
    info.compare = cmp;
    info.data_print = item_print;
    info.key_print = item_print;
    info.data_dealloc = nothing;
    info.key_dealloc = nothing;
    size_t k1 = 1, k2 = 3, k3 = 5, k4 = 0, k5 = 9, k6 = 11;
    size_t data = 1;
    Item *i1, *i2, *i3, *i4, *i5, *i6;
    table_init(&table, 10, &info);
    item_make(&i1, &k1, &data);
    item_make(&i2, &k2, &data);
    item_make(&i3, &k3, &data);
    item_make(&i4, &k4, &data);
    item_make(&i5, &k5, &data);
    item_make(&i6, &k6, &data);
    table_insert(&table, i1);
    table_insert(&table, i2);
    table_insert(&table, i4);
    table_insert(&table, i5);
    table_insert(&table, i3);
    table_insert(&table, i6);
    table_print(&table);
    table_dealloc(&table);
    return 0;
}