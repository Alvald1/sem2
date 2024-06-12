#ifndef LAB3A_3A_MAIN_H_
#define LAB3A_3A_MAIN_H_

#include <stddef.h>

#include "lib/info.h"

#ifdef COMMON
#include "lib/table.h"
#endif

#ifdef ITERATOR
#include "lib/table_iterator.h"
#endif

Foo insert(Table* table);
Foo rem(Table* table, Info* info);
Foo search(Table* table, Info* info);
void print(Table* table);
Foo file(Table** table, Info* info);
Foo task(Table* table);

#endif