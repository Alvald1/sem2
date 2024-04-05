#include "main.h"
#include <stdio.h>
#include "lib/code_status.h"
#include "lib/info.h"
#include "lib/item.h"
#include "lib/table.h"

void
nothing(void* data) {
    (void)data;
}

void
print(void* data) {
    printf("%zu\t", *((size_t*)data));
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
    Item* item;
    table_search(&table, &k2, &item);
    item_print(&info, item);
    item_dealloc(&info, item);
    table_dealloc(&table);
    return 0;
}