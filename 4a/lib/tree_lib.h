#ifndef LAB4A_4A_LIB_TREE_LIB_H_
#define LAB4A_4A_LIB_TREE_LIB_H_

#include "tree.h"

struct _tree {
    void *key, *data;
    struct _tree *left, *right, *parent;
};

#endif