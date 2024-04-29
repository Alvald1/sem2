#ifndef LAB3B_3B_LIB_ITEM_H_
#define LAB3B_3B_LIB_ITEM_H_

#include "info.h"

typedef struct _item {
    char busy;
    void* key;
    void* data;
} Item;

Foo item_dealloc(Item* item, Info* info);

#endif