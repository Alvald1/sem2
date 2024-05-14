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

Foo
tree_insert(Tree* root, void* key, void* data, void** result) {
    if (__tree_valid(root) == BAD_DATA || key == NULL || data == NULL || result == NULL) {
        return BAD_DATA;
    }
    if (root->key == NULL) {
        __tree_fill(root, key, data);
        return OK;
    }
    fptr_compare compare = root->info->compare;
    Tree* parent = NULL;
    Tree* node = NULL;
    Tree* current = root;
    Foo call_back = OK;
    Tree* duplicate = NULL;
    switch (tree_search(root, key, &duplicate)) {
        case OK:
            *result = duplicate->data;
            duplicate->data = data;
            return DUPLICATE;
        case NOT_FOUND: break;
        default: return BAD_DATA;
    }
    while (current != NULL) {
        parent = current;
        if ((*compare)(key, current->key) == LESS) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if ((call_back = tree_init(&node, root->info)) != OK) {
        return call_back;
    }
    __tree_fill(node, key, data);
    node->parent = parent;
    if ((*compare)(key, parent->key) == LESS) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    return OK;
}

Foo
tree_delete(Tree** root, void* key) {
    if (__tree_valid(*root) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    Foo call_back = OK;
    Tree *successor = NULL, *result = NULL;
    if ((call_back = tree_search(*root, key, &result)) != OK) {
        return call_back;
    }
    if (result->left == NULL) {
        __tree_transplant(root, result, result->right);
    } else if (result->right == NULL) {
        __tree_transplant(root, result, result->left);
    } else {
        successor = __tree_minimum(result->right);
        if (successor != result->right) {
            __tree_transplant(root, successor, successor->right);
            successor->right = result->right;
            successor->right->parent = successor;
        }
        __tree_transplant(root, result, successor);
        successor->left = result->left;
        successor->left->parent = successor;
    }
    __tree_dealloc(result);
    return OK;
}

Tree*
tree_maximum(Tree* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

Foo
tree_search(Tree* root, void* key, Tree** result) {
    if (__tree_valid(root) == BAD_DATA || key == NULL || result == NULL) {
        return BAD_DATA;
    }
    fptr_compare compare = root->info->compare;
    Compare call_back = EQUAL;
    while (root != NULL && (call_back = (*compare)(key, root->key)) != EQUAL) {
        if (call_back == LESS) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    *result = root;
    if (root) {
        return OK;
    }
    return NOT_FOUND;
}

Foo
tree_dealloc(Tree* root) {
    return __tree_postorder(root, __tree_dealloc);
}

Foo
tree_print(Tree* root) {
    return __tree_postorder(root, __print);
}
