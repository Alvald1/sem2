#ifndef LAB3A_3A_LIB_ITERATOR_LIB_H_
#define LAB3A_3A_LIB_ITERATOR_LIB_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"
#include "iterator.h"
#include "table.h"

Foo __iterator_valid(Iterator* iterator);
Foo __iterator_init(Iterator** iterator, Info* info);
Foo __iterator_get(Table* table, Iterator** iterator, size_t pos);

#endif