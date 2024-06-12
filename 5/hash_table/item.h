#ifndef LAB3B_3B_LIB_ITEM_H_
#define LAB3B_3B_LIB_ITEM_H_

#include "code_status.h"
#include "info.h"

typedef struct _item Item;

void item_print(Item* item, Hash_Info* info);
Hash_Foo item_dealloc(Item* item, Hash_Info* info);
void* get_key(Item* item);
void* get_data(Item* item);

#endif