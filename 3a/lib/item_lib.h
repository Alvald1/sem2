#ifndef __ITEM_LIB__
#define __ITEM_LIB__

#include "item.h"

Foo __item_valid(Item* item);

Foo __item_make(Item** item, void* key, void* data);

#endif