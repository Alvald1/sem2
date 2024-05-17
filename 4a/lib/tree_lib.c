#include "tree_lib.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"

void
__node_dealloc(Tree* tree, Node* node) {
    tree->info->key_dealloc(node->key);
    tree->info->data_dealloc(node->data);
    free(node);
}

Node*
__node_minimum(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

void
__node_print(Tree* tree, Node* node) {
    tree->info->key_print(node->key);
    tree->info->data_print(node->data);
    printf("\n");
}

Foo
__node_init(Node** node, void* key, void* data) {
    *node = (Node*)calloc(1, sizeof(Node));
    if (*node == NULL) {
        return BAD_ALLOC;
    }
    (*node)->key = key;
    (*node)->data = data;
    return OK;
}

Foo
__tree_valid(Tree* tree) {
    if (tree == NULL || __info_valid(tree->info) == BAD_DATA) {
        return BAD_DATA;
    }
    return OK;
}

void
__tree_transplant(Tree* tree, Node* u, Node* v) {
    if (u->parent == NULL) {
        tree->root = v;
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
__tree_postorder(Tree* tree, fptr_action action) {
    Foo return_code = OK;
    Node* current = NULL;
    Node *previous = NULL, *predecessor = NULL;
    Node *successor = NULL, *temp = NULL;
    if (tree->root == NULL) {
        return OK;
    }
    if ((return_code = __node_init(&current, NULL, NULL)) != OK) {
        return return_code;
    }
    current->left = tree->root;
    while (current != NULL) {
        if (current->left == NULL) {
            current = current->right;
        } else {
            predecessor = current->left;
            while (predecessor->right != NULL && predecessor->right != current) {
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
                    (*action)(tree, previous);
                    previous = temp;
                }
                current = successor;
                current = current->right;
            }
        }
    }
    __node_dealloc(tree, successor);
    return OK;
}
