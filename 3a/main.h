#ifndef LAB3A_3A_MAIN_H_
#define LAB3A_3A_MAIN_H_

#include <stddef.h>

#include "lib/info.h"
#include "lib/table.h"

Foo insert(Table* table);
Foo rem(Table* table);
Foo search(Table* table, Info* info);
void print(Table* table);
Foo file(Table** table, Info* info);
Foo task(Table* table);

#endif