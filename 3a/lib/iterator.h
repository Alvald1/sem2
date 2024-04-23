#ifndef LAB3A_3A_LIB_ITERATOR_H_
#define LAB3A_3A_LIB_ITERATOR_H_

#include "info.h"
#include "item.h"

typedef struct _table Table;

typedef struct _iterator {
    Item* item;
    Info* info;
} Iterator;

Foo iterator_make(Iterator** iterator, Info* info, void* key, void* data);
Foo iterator_compare(Iterator* left, Iterator* right);
Foo iterator_front(Table* table, Iterator** iterator);
Foo iterator_back(Table* table, Iterator** iterator);
Foo iterator_next(Table* table, Iterator* iterator, Iterator** next);
void iterator_dealloc(Iterator* iterator);

#endif