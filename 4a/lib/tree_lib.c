#include "tree_lib.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"

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

void
__tree_dealloc(Tree* root) {
    root->info->key_dealloc(root->key);
    root->info->data_dealloc(root->data);
    free(root);
}

Tree*
__tree_minimum(Tree* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

void
__print(Tree* root) {
    root->info->key_print(root->key);
    root->info->data_print(root->data);
    printf("\n");
}

Foo
__tree_postorder(Tree* root, fptr_action action) {
    if (root == NULL) {
        return OK;
    }
    Foo call_back = OK;
    Tree* current = NULL;
    if ((call_back = tree_init(&current, root->info)) != OK) {
        return call_back;
    }
    Tree* predecessor = NULL;
    Tree* previous = NULL;
    Tree* successor = NULL;
    Tree* temp = NULL;
    current->left = root;
    while (current) {
        if (current->left == NULL) {
            current = current->right;
        } else {
            predecessor = current->left;
            while (predecessor->right && predecessor->right != current) {
                predecessor = predecessor->right;
            }
            if (predecessor->right == NULL) {
                predecessor->right = current;
                current = current->left;
            } else {
                predecessor->right = NULL;
                successor = current;
                current = current->left;
                previous = NULL;
                while (current != NULL) {
                    temp = current->right;
                    current->right = previous;
                    previous = current;
                    current = temp;
                }
                while (previous != NULL) {
                    temp = previous->right;
                    previous->right = current;
                    current = previous;
                    (*action)(previous);
                    previous = temp;
                }
                current = successor;
                current = current->right;
            }
        }
    }
    __tree_dealloc(successor);
    return OK;
}
