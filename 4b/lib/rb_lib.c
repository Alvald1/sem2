#include "rb_lib.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"

#define COUNT 10

const char* colors[] = {"red", "black"};

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
__node_init(RB* rb, Node** node, void* key, void* data) {
    *node = (Node*)malloc(sizeof(Node));
    List* list = (List*)malloc(sizeof(List));
    if (*node == NULL || list == NULL) {
        free(*node);
        free(list);
        return BAD_ALLOC;
    }
    list->data = data;
    list->next = NULL;
    list->release = 1;
    (*node)->left = rb->nil;
    (*node)->right = rb->nil;
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
    Node* current = rb->nil;
    Node *previous = rb->nil, *predecessor = rb->nil;
    Node *successor = rb->nil, *temp = rb->nil;
    if (rb->root == rb->nil) {
        return OK;
    }
    if ((return_code = __node_init(rb, &current, NULL, NULL)) != OK) {
        return return_code;
    }
    current->left = rb->root;
    while (current != rb->nil) {
        if (current->left == rb->nil) {
            current = current->right;
        } else {
            predecessor = current->left;
            while (predecessor->right != rb->nil && predecessor->right != current) {
                predecessor = predecessor->right;
            }
            if (predecessor->right == rb->nil) {
                predecessor->right = current;
                current = current->left;
            } else {
                predecessor->right = rb->nil;
                successor = current;
                current = current->left;
                previous = rb->nil;
                while (current != rb->nil) {
                    temp = current->right;
                    current->right = previous;
                    previous = current;
                    current = temp;
                }
                while (previous != rb->nil) {
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
__left_rotate(RB* rb, Node* node) {
    Node* current = node->right;
    node->right = current->left;
    if (current->left != rb->nil) {
        current->left->parent = node;
    }
    current->parent = node->parent;
    if (node->parent == rb->nil) {
        rb->root = current;
    } else if (node == node->parent->left) {
        node->parent->left = current;
    } else {
        node->parent->right = current;
    }
    current->left = node;
    node->parent = current;
}

void
__right_rotate(RB* rb, Node* node) {
    Node* current = node->left;
    node->left = current->right;
    if (current->right != rb->nil) {
        current->left->parent = node;
    }
    current->parent = node->parent;
    if (node->parent == rb->nil) {
        rb->root = current;
    } else if (node == node->parent->left) {
        node->parent->left = current;
    } else {
        node->parent->right = current;
    }
    current->right = node;
    node->parent = current;
}

void
__rb_insert_fixup(RB* rb, Node* node) {
    Node* uncle = NULL;
    while (node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            uncle = node->parent->parent->right;
            if (uncle->color == RED) {       //дядя - красный
                node->parent->color = BLACK; //средний уровень - черный
                uncle->color = BLACK;
                node->parent->parent->color = RED; //остальные - красный
                node = node->parent->parent;
            } else {                               //дядя - черный
                if (node == node->parent->right) { //справа
                    node = node->parent;           //перепрыгнуть на родителя
                    __left_rotate(rb, node);       //левый поворот от себя
                } //слева
                node->parent->color = BLACK; //смена цветов деда и родителя
                node->parent->parent->color = RED;
                __right_rotate(rb, node->parent->parent); //правый поворот от деда
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
    }
    rb->root->color = BLACK;
}