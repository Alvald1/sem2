#ifndef __TABLE_LIB__
#define __TABLE_LIB__

#include "table.h"

int __table_valid(Table* table);

size_t __table_cnt(Table* table);

size_t __table_capacity(Table* table);

int __table_search(Table* table, void* key, size_t* result);

void __set_cnt(Table* table, size_t cnt);

int __table_insert(Table* table, Item* item);

void* __table_data(Table* table, size_t pos);

#endif