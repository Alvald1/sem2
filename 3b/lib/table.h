#ifndef LAB3B_3B_LIB_TABLE_H_
#define LAB3B_3B_LIB_TABLE_H_

#include <stddef.h>

#include "code_status.h"
#include "info.h"
#include "item.h"

typedef struct _table Table;

Foo table_init(Table** table, size_t capacity, Info* info);
Foo table_insert(Table** table, void* key, void* data);
Foo table_remove(Table* table, void* key);
Foo table_search(Table* table, void* key, Item** result);
void table_print(Table* table);
Foo table_import(Table** table, Info* info, const char* file_name);
Foo table_export(Table* table, const char* file_name);
void table_dealloc(Table* table);

#endif