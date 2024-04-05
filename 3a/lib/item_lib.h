#ifndef __ITEM_LIB__
#define __ITEM_LIB__

#include "item.h"

int __item_valid(Item* item);

int __item_make(Item** item, void* key, void* data);

#endif