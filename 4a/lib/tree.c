#include "tree.h"

#include <stdlib.h>

#include "info_lib.h"
#include "tree_lib.h"

Foo
tree_init(Tree** root, Info* info) {
    if (root == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *root = (Tree*)calloc(1, sizeof(Tree));
    if (*root == NULL) {
        return BAD_ALLOC;
    }
    (*root)->info = info;
    return OK;
}

void
__tree_fill(Tree* root, void* key, void* data) {
    root->key = key;
    root->data = data;
}

Foo
__tree_valid(Tree* root) {
    if (root == NULL || __info_valid(root->info) == BAD_DATA) {
        return BAD_DATA;
    }
    return OK;
}

Foo
tree_insert(Tree* root, void* key, void* data, void** result) {
    if (__tree_valid(root) == BAD_DATA || key == NULL || data == NULL || result == NULL) {
        return BAD_DATA;
    }
    fptr_compare* compare = root->info->compare;
    Tree* parent = NULL;
    Tree* node = NULL;
    Tree* current = root;
    Foo call_back = OK;
    while (current != NULL) {
        parent = current;
        if ((*compare)(key, current->key) == LESS) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (parent == NULL) {
        __tree_fill(root, key, data);
    } else {
        if ((call_back = tree_init(&node, root->info)) != OK) {
            return call_back;
        }
        __tree_fill(node, key, data);
        node->parent = parent;
        if ((*compare)(key, parent->key) == LESS) {
            parent->left == node;
        } else {
            parent->right = node;
        }
    }
    return OK;
}
