#ifndef LAB3B_3B_LIB_TABLE_LIB_H_
#define LAB3B_3B_LIB_TABLE_LIB_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"
#include "item.h"

struct _table {
    Item* items;
    size_t capacity, size, step;
    Info* info;
};

void __table_fill(Table* table, size_t capacity, size_t size, Info* info, Item* items);
Foo __table_expand(Table* table);
Foo __table_valid(Table* table);
Foo __table_search(Table* table, void* key, size_t* result);

#endif