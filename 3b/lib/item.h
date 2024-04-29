#ifndef LAB3B_3B_LIB_ITEM_H_
#define LAB3B_3B_LIB_ITEM_H_

typedef struct _item {
    char busy;
    void* key;
    void* data;
} Item;

#endif