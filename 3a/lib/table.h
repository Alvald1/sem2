#ifndef LAB3A_3A_LIB_TABLE_H_
#define LAB3A_3A_LIB_TABLE_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"
#include "item.h"

typedef struct _table Table;

Foo table_remove_by_range(Table* table, void* left, void* right);

Foo table_init(Table** table, size_t capacity, Info* info);

Foo table_search(Table* table, void* key, Item** result);

Foo table_print(Table* table);

Foo table_insert(Table* table, void* key, void* data);

Foo table_dealloc(Table* table);

Foo table_remove(Table* table, void* key);

#endif