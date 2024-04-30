#include <stdio.h>

#include <stdlib.h>

#include "lib/info.h"
#include "lib/table.h"

int
compare(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return 1;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return 0;
    } else {
        return -1;
    }
}

void
print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

void
dealloc(void* data) {
    free(data);
}

size_t*
gen_number(size_t value) {
    size_t* number = malloc(sizeof(size_t));
    *number = value;
    return number;
}

int
main() {
    Table* table = NULL;
    Info* info = NULL;
    void* tmp = NULL;
    info_init(&info, compare, dealloc, dealloc, print, print, sizeof(size_t));
    table_init(&table, 10, info);
    table_insert(table, gen_number(10), gen_number(10));
    table_print(table);
    table_insert(table, gen_number(15), gen_number(15));
    table_print(table);
    table_insert(table, gen_number(16), gen_number(16));
    table_print(table);
    table_insert(table, gen_number(1), gen_number(1));
    table_print(table);
    table_remove(table, (tmp = gen_number(1)));
    free(tmp);
    table_print(table);
    table_dealloc(table);
    info_dealloc(info);
    return 0;
}
