#ifndef __ITEM__
#define __ITEM__

typedef struct _item {
    void* key;
    void* data;
} Item;

#include "code_status.h"
#include "info.h"

Foo item_dealloc(Info* info, Item* item);

Foo item_print(Info* info, Item* item);

#endif