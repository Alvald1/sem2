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

Foo table_init(Table* table, size_t capacity, Info* info);

Foo table_search(Table* table, void* key, Item** result);

Foo table_print(Table* table);

Foo table_insert(Table* table, void* key, void* data);

Foo table_dealloc(Table* table);

Foo table_remove(Table* table, void* key);

#endif