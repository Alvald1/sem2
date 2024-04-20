#ifndef LAB3A_3A_LIB_TABLE_ITERATOR_H_
#define LAB3A_3A_LIB_TABLE_ITERATOR_H_

#include "info.h"
#include "item.h"
#include "table.h"

typedef struct _iterator {
    Item* item;
    Info* info;
} Iterator;

Foo iterator_front(Table* table, Iterator** iterator);
Foo iterator_back(Table* table, Iterator** iterator);
Foo iterator_next(Table* table, Iterator* iterator, Iterator** next);
void iterator_dealloc(Iterator* iterator);
Foo iterator_compare(Iterator* left, Iterator* right);
Foo iterator_insert(Table* table, void* key, void* data, Iterator** iterator);
Foo iterator_remove(Table* table, Iterator* iterator, Iterator** next);
Foo iterator_search(Table* table, void* key, Iterator** iterator);

#endif