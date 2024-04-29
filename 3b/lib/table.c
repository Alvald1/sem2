#include "table.h"

struct _table {
    Item* items;
    size_t capacity, size;
    Info* info;
} Table;
