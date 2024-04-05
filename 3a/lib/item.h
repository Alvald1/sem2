#ifndef __ITEM__
#define __ITEM__

typedef struct _item {
    void* key;
    void* data;
} Item;

#include "info.h"

int item_dealloc(Info* info, Item* item);

int item_print(Info* info, Item* item);

#endif