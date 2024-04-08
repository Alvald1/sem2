#ifndef __TABLE_LIB__
#define __TABLE_LIB__

#include "table.h"

int __table_valid(Table* table);

size_t __table_size(Table* table);

size_t __table_capacity(Table* table);

int __table_search(Table* table, void* key, size_t* result);

void __set_size(Table* table, size_t size);

int __table_insert(Table* table, Item* item);

void* __table_data(Table* table, size_t pos);

#endif