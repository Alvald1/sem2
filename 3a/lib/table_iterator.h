#ifndef LAB3A_3A_LIB_TABLE_ITERATOR_H_
#define LAB3A_3A_LIB_TABLE_ITERATOR_H_

#include "info.h"
#include "item.h"
#include "iterator.h"
#include "table.h"

Foo table_iterator_insert(Table* table, void* key, void* data, Iterator** iterator);
Foo table_iterator_remove(Table* table, Iterator* iterator, Iterator** next);
Foo table_iterator_search(Table* table, void* key, Iterator** iterator);

#endif