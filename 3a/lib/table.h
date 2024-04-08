#ifndef __TABLE__
#define __TABLE__

#include <stddef.h>
#include "info.h"
#include "item.h"

typedef struct _table {
    Item** items;
    size_t capacity;
    size_t size;
    Info* info;
} Table;

int table_init(Table* table, size_t capacity, Info* info);

int table_search(Table* table, void* key, Item** result);

int table_print(Table* table);

int table_insert(Table* table, void* key, void* data);

int table_dealloc(Table* table);

int table_remove(Table* table, void* key);

#endif