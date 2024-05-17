#include "tree.h"

#include <stdlib.h>

#include "info_lib.h"
#include "tree_lib.h"

Foo
tree_init(Tree** tree, Info* info) {
    if (tree == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *tree = (Tree*)calloc(1, sizeof(Tree));
    if (*tree == NULL) {
        return BAD_ALLOC;
    }
    (*tree)->info = info;
    return OK;
}

Foo
tree_insert(Tree* tree, void* key, void* data, void** result) {
    if (__tree_valid(tree) == BAD_DATA || key == NULL || data == NULL || result == NULL) {
        return BAD_DATA;
    }
    Foo return_code = OK;
    Node *node_result = NULL, *node = NULL;
    switch (tree_search(tree, key, &node_result)) {
        case OK:
            *result = node_result->data;
            node_result->data = data;
            return DUPLICATE;
        case NOT_FOUND:
            if ((return_code = __node_init(&node, key, data)) != OK) {
                return return_code;
            }
            if (node_result == NULL) {
                tree->root = node;
            } else if ((*tree->info->compare)(key, node_result->key) == LESS) {
                node_result->left = node;
            } else {
                node_result->right = node;
            }
            node->parent = node_result;
            return OK;
        default: return BAD_DATA;
    }
}

Foo
tree_delete(Tree* tree, void* key) {
    if (__tree_valid(tree) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    Foo return_code = OK;
    Node *successor = NULL, *result = NULL;
    if ((return_code = tree_search(tree, key, &result)) != OK) {
        return return_code;
    }
    if (result->left == NULL) {
        __tree_transplant(tree, result, result->right);
    } else if (result->right == NULL) {
        __tree_transplant(tree, result, result->left);
    } else {
        successor = __node_minimum(result->right);
        if (successor != result->right) {
            __tree_transplant(tree, successor, successor->right);
            successor->right = result->right;
            successor->right->parent = successor;
        }
        __tree_transplant(tree, result, successor);
        successor->left = result->left;
        successor->left->parent = successor;
    }
    __node_dealloc(tree, result);
    return OK;
}

Node*
tree_maximum(Tree* tree) {
    Node* root = tree->root;
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}

Foo
tree_search(Tree* tree, void* key, Node** result) {
    if (__tree_valid(tree) == BAD_DATA || key == NULL || result == NULL) {
        return BAD_DATA;
    }
    fptr_compare compare = tree->info->compare;
    Compare return_code = EQUAL;
    Node *root = tree->root, *parent = NULL;
    while (root != NULL && (return_code = (*compare)(key, root->key)) != EQUAL) {
        parent = root;
        if (return_code == LESS) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    if (root != NULL) {
        *result = root;
        return OK;
    }
    *result = parent;
    return NOT_FOUND;
}

Foo
tree_dealloc(Tree* tree) {
    Foo return_code = __tree_postorder(tree, __node_dealloc);
    if (return_code != OK) {
        return return_code;
    }
    free(tree);
    return OK;
}

Foo
tree_print(Tree* tree) {
    return __tree_postorder(tree, __node_print);
}
