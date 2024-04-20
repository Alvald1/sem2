#ifndef LAB3A_3A_LIB_TABLE_LIB_H_
#define LAB3A_3A_LIB_TABLE_LIB_H_

#include "table.h"

struct _table {
    Item** items;
    size_t capacity;
    size_t size;
    Info* info;
};

Foo __table_valid(Table* table);
size_t __table_size(Table* table);
size_t __table_capacity(Table* table);
Foo __table_search(Table* table, void* key, size_t* result);
void __set_size(Table* table, size_t size);
Foo __table_insert(Table* table, Item* item, size_t* result);
Foo __table_insert_(Table* table, void* key, void* data, size_t* result);
void* __table_data(Table* table, size_t pos);
Item* __table_item(Table* table, size_t pos);

#endif