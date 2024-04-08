#ifndef __ITEM__
#define __ITEM__

typedef struct _item {
    void* key;
    void* data;
} Item;

#include "info.h"

Foo item_dealloc(Info* info, Item* item);

Foo item_print(Info* info, Item* item);

#endif