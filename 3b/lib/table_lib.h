#ifndef LAB3B_3B_LIB_TABLE_LIB_H_
#define LAB3B_3B_LIB_TABLE_LIB_H_

#include <stddef.h>

#include "info.h"
#include "item.h"

struct _table {
    Item* items;
    size_t capacity, size;
    Info* info;
};

Foo __table_valid(Table* table);

#endif