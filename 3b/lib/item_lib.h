#ifndef LAB3B_3B_LIB_ITEM_LIB_H_
#define LAB3B_3B_LIB_ITEM_LIB_H_

#include "code_status.h"
#include "info.h"
#include "item.h"

struct _item {
    char busy;
    void* key;
    void* data;
};

Foo __item_valid(Item* item);
Foo __item_fill(void* key, void* data, Item* item);
Foo __item_dealloc(Item* item, Info* info);
Foo __item_copy(Item* dest, Item* src, Info* info);

#endif