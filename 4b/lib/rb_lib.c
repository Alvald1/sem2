#include "rb_lib.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"

#define COUNT 10

Foo
__list_push(List** list, void* data) {
    List* new = (List*)malloc(sizeof(List));
    if (new == NULL) {
        return BAD_ALLOC;
    }
    new->data = data;
    new->next = *list;
    new->release = (*list)->release + 1;
    *list = new;
    return OK;
}

Node*
__node_minimum(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

Foo
__node_init(Node** node, void* key, void* data) {
    *node = (Node*)calloc(1, sizeof(Node));
    List* list = (List*)malloc(sizeof(List));
    if (*node == NULL || list == NULL) {
        free(*node);
        free(list);
        return BAD_ALLOC;
    }
    list->data = data;
    list->next = NULL;
    list->release = 1;
    (*node)->key = key;
    (*node)->list = list;
    (*node)->color = RED;
    return OK;
}

Foo
__rb_dealloc(RB* rb) {
    return __rb_postorder(rb, node_dealloc);
}

Foo
__rb_valid(RB* rb) {
    if (rb == NULL || __info_valid(rb->info) == BAD_DATA) {
        return BAD_DATA;
    }
    return OK;
}

void
__rb_2D(RB* rb, Node* node, size_t space) {
    if (node == NULL) {
        return;
    }
    space += COUNT;
    __rb_2D(rb, node->right, space);
    printf("\n");
    for (size_t i = COUNT; i < space; ++i) {
        printf(" ");
    }
    rb->info->key_print(node->key);
    __rb_2D(rb, node->left, space);
}

void
__rb_desc(RB* rb, Node* node) {
    if (node == NULL) {
        return;
    }
    __rb_desc(rb, node->right);
    node_print(node, rb);
    __rb_desc(rb, node->left);
}

void
__rb_transplant(RB* rb, Node* u, Node* v) {
    if (u->parent == NULL) {
        rb->root = v;
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
__rb_postorder(RB* rb, fptr_action action) {
    Foo return_code = OK;
    Node* current = NULL;
    Node *previous = NULL, *predecessor = NULL;
    Node *successor = NULL, *temp = NULL;
    if (rb->root == NULL) {
        return OK;
    }
    if ((return_code = __node_init(&current, NULL, NULL)) != OK) {
        return return_code;
    }
    current->left = rb->root;
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
                    (*action)(previous, rb);
                    previous = temp;
                }
                current = successor;
                current = current->right;
            }
        }
    }
    node_dealloc(successor, rb);
    return OK;
}

void
__rb_insert_fixup(RB* rb, Node* node) {
    Node* uncle = NULL;
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    __left_rotate(rb, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                __right_rotate(rb, node->parent->parent);
            }
        } else {
            uncle = node->parent->parent->left;
            if (uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    __right_rotate(rb, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                __left_rotate(rb, node->parent->parent);
            }
        }
        rb->root->color = BLACK;
    }
}