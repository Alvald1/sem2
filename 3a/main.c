#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"

typedef int (*fptr_compare)(void* left, void* right);
typedef int (*fptr_print)(void* data);

typedef struct _info {
    fptr_compare compare;
    fptr_print key_print, data_print;
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
    if (table_valid(table) == BAD_DATA || info_valid(info) == BAD_DATA) {
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

Item*
searchR(Item** items, fptr_compare compare, long left, long right, void* key) {
    if (left > right) {
        return NULL;
    }
    long middle = left + (right - left) / 2;
    int cmp = (*compare)(items[middle]->key, key);
    if (cmp == 0) {
        return items[middle];
    }
    if (left == right) {
        return NULL;
    }
    if (cmp == 1) {
        return searchR(items, compare, left, middle - 1, key);
    } else {
        return searchR(items, compare, middle + 1, right, key);
    }
}

Item*
table_search(Table* table, void* key) {
    if (table_valid(table) == BAD_DATA) {
        return NULL;
    }
    Item** items = table->items;
    fptr_compare compare = table->info->compare;
    return searchR(items, compare, 0, table_cnt(table) - 1, key);
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
    size_t cnt = table_cnt(cnt);
    if (cnt) {
        printf("key\tdata\n");
    }
    for (size_t i = 0; i < cnt; ++i) {}
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
    if (table_search(table, key) != NULL) {
        return BAD_KEY;
    }
    for (i = cnt; i > 0 && (*compare)(items[i - 1]->key, key) == 1; --i) {
        items[i] = items[i - 1];
    }
    items[i] = item;
    set_cnt(table, cnt + 1);
    return OK;
}

int
main() {
    Table table;
    Info info;
    info.compare = cmp;
    size_t k1 = 1, k2 = 2, k3 = 2, k4 = 4, k5 = 5, k6 = 6;
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
    return 0;
}