#ifndef LAB3B_3B_LIB_TABLE_H_
#define LAB3B_3B_LIB_TABLE_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"
#include "item.h"

typedef struct _table Table;

Hash_Foo table_init(Table** table, size_t capacity, Hash_Info* info);
Hash_Foo table_insert(Table* table, void* key, void* data);
Hash_Foo table_remove(Table* table, void* key);
Hash_Foo table_search(Table* table, void* key, Item** result);
Hash_Foo table_print(Table* table);
Hash_Foo table_dealloc(Table* table);

#endif