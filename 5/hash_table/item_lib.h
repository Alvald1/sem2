#ifndef LAB3B_3B_LIB_ITEM_LIB_H_
#define LAB3B_3B_LIB_ITEM_LIB_H_

#include "code_status.h"
#include "info.h"
#include "item.h"

struct _item {
    Hash_Status status;
    void* key;
    void* data;
};

Hash_Foo __item_valid(Item* item);
Hash_Foo __item_fill(void* key, void* data, Item* item);
Hash_Foo __item_dealloc(Item* item, Hash_Info* info);
Hash_Foo __item_copy(Item* dest, Item* src, Hash_Info* info);

#endif