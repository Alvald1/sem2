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
    Foo foo_call_back = OK;
    Compare compare_call_back = EQUAL;
    while (current != NULL) {
        parent = current;
        if ((compare_call_back = (*compare)(key, current->key)) == EQUAL) {
            *result = current->data;
            current->data = data;
            return DUPLICATE;
        } else if (compare_call_back == LESS) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (parent == NULL) {
        __tree_fill(root, key, data);
    } else {
        if ((foo_call_back = tree_init(&node, root->info)) != OK) {
            return foo_call_back;
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

void
__tree_transplant(Tree** root, Tree* u, Tree* v) {
    if (u->parent == NULL) {
        *root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

Foo
tree_delete(Tree* root, void* key, Tree** result) {
    if (__tree_valid(root) == BAD_DATA || key == NULL || result == NULL) {
        return BAD_DATA;
    }
    Tree* successor = NULL;
    tree_search(root, key, result);
    if ((*result)->left == NULL) {
        __tree_transplant(&root, *result, (*result)->right);
    } else if ((*result)->right == NULL) {
        __tree_transplant(&root, *result, (*result)->left);
    } else {
        successor = __tree_minimum((*result)->right);
        if (successor != (*result)->right) {
            __tree_transplant(&root, successor, successor->right);
            successor->right = (*result)->right;
            successor->right->parent = successor;
        }
        __tree_transplant(&root, *result, successor);
        successor->left = (*result)->left;
        successor->left->parent = successor;
    }
    return OK;
}

Tree*
tree_maximum(Tree* root) {
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

Tree*
__tree_minimum(Tree* root) {
    while (root->left != NULL) {
        root = root->left;
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
    return OK;
}