#ifndef LAB3A_3A_LIB_ITEM_LIB_H_
#define LAB3A_3A_LIB_ITEM_LIB_H_

#include "item.h"

Foo __item_valid(Item* item);

Foo __item_make(Item** item, void* key, void* data);

#endif