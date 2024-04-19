#ifndef LAB3A_3A_LIB_TABLE_ITERATOR_H_
#define LAB3A_3A_LIB_TABLE_ITERATOR_H_

#include "info.h"
#include "item.h"
#include "table.h"

typedef struct _iterator {
    Item* item;
    Info* info;
} Iterator;

Foo iterator_first(Table* table, Iterator** iterator);

#endif