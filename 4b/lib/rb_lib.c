#include "rb_lib.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"

#define COUNT 10

const char* colors[] = {"red", "black"};

void
__node_dealloc(RB* rb, Node* node) {
    rb->info->key_dealloc(node->key);
    List *current = node->list, *temp = NULL;
    while (current != NULL) {
        rb->info->data_dealloc(current->data);
        temp = current;
        current = current->next;
        free(temp);
    }
    free(node);
}

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
__node_minimum(RB* rb, Node* root) {
    while (root->left != rb->nil) {
        root = root->left;
    }
    return root;
}

void
__node_print(RB* rb, Node* node, const char* left, const char* right) {
    Compare compare_left, compare_right;
    compare_left = rb->info->compare(node->key, (void*)left);
    compare_right = rb->info->compare(node->key, (void*)right);
    if (compare_left == LESS || compare_right == MORE) {
        node_print(rb, node);
    }
}

void
__rb_out_of_range(RB* rb, Node* node, const char* left, const char* right) {
    if (node == rb->nil) {
        return;
    }
    __rb_out_of_range(rb, node->left, left, right);
    __node_print(rb, node, left, right);
    __rb_out_of_range(rb, node->right, left, right);
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

Count
__delete_release(RB* rb, List** list, size_t release) {
    if ((*list)->next == NULL) {
        return ONE;
    }
    List *current = NULL, *temp = NULL;
    if ((*list)->release == release) {
        rb->info->data_dealloc((*list)->data);
        current = (*list);
        *list = (*list)->next;
        free(current);
        return MANY;
    }
    current = *list;
    while (current->next != NULL) {
        if ((*list)->next->release == release) {
            rb->info->data_dealloc((*list)->next->data);
            temp = (*list)->next;
            (*list)->next = temp->next;
            free(temp);
            return MANY;
        }
        current = current->next;
    }
    return _NOT_FOUND;
}

Foo
__rb_dealloc(RB* rb) {
    return __rb_postorder(rb, __node_dealloc);
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
    if (node == rb->nil) {
        return;
    }
    space += COUNT;
    __rb_2D(rb, node->right, space);
    printf("\n\n");
    for (size_t i = COUNT; i < space; ++i) {
        printf(" ");
    }
    rb->info->key_print(node->key);
    __rb_2D(rb, node->left, space);
}

void
__rb_transplant(RB* rb, Node* u, Node* v) {
    if (u->parent == rb->nil) {
        rb->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

Foo
__rb_postorder(RB* rb, fptr_action action) {
    Foo return_code = OK;
    Node* current = NULL;
    Node *previous = NULL, *predecessor = NULL;
    Node *successor = NULL, *temp = NULL;
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
                previous = NULL;
                while (current != rb->nil) {
                    temp = current->right;
                    current->right = previous;
                    previous = current;
                    current = temp;
                }
                while (previous != NULL) {
                    temp = previous->right;
                    previous->right = current;
                    current = previous;
                    (*action)(rb, previous);
                    previous = temp;
                }
                current = successor;
                current = current->right;
            }
        }
    }
    __node_dealloc(rb, successor);
    return OK;
}

void
__rb_inorder(RB* rb, fptr_action action) {
    Node *current = NULL, *predecessor = NULL;
    if (rb->root == rb->nil) {
        return;
    }
    current = rb->root;
    while (current != rb->nil) {
        if (current->left == rb->nil) {
            (*action)(rb, current);
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
                (*action)(rb, current);
                current = current->right;
            }
        }
    }
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
        if (node->parent == node->parent->parent->left) { //левый сын?
            uncle = node->parent->parent->right;
            if (uncle->color == RED) {       //дядя - красный?
                node->parent->color = BLACK; //средний уровень - черный
                uncle->color = BLACK;
                node->parent->parent->color = RED; //остальные - красный
                node = node->parent->parent;
            } else {                               //дядя - черный?
                if (node == node->parent->right) { //справа?
                    node = node->parent;           //перепрыгнуть на родителя
                    __left_rotate(rb, node);       //левый поворот от себя
                } //слева?
                node->parent->color = BLACK; //смена цветов деда и родителя
                node->parent->parent->color = RED;
                __right_rotate(rb, node->parent->parent); //правый поворот от деда
            }
        } else { //правый сын?
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

void
__rb_delete_fixup(RB* rb, Node* node) {
    Node* brother = NULL;
    while (node != rb->root && node->color == BLACK) {
        if (node == node->parent->left) { //левый сын?
            brother = node->parent->right;
            if (brother->color == RED) { //брат-красный?
                brother->color = BLACK;  //смена цветов брата и родителя
                node->parent->color = RED;
                __left_rotate(rb, node->parent); //левый поворот от родителя
                brother = node->parent->right;
            }
            if (brother->left->color == BLACK && brother->right->color == BLACK) { //брат-черный? дети-черные?
                brother->color = RED;                                              //брат-красный
                node = node->parent; //перепрыгнуть на родителя
            } else {
                if (brother->right->color == BLACK) { //левый ребенок-красный?
                    brother->left->color = BLACK; //смена цветов брата и левого ребенка
                    brother->color = RED;
                    __right_rotate(rb, brother); //правый поворот от брата
                    brother = node->parent->right;
                } //правый ребенок-красный?
                brother->color = node->parent->color; //брат-цвет родителя
                node->parent->color = BLACK;          //родитель-черный
                brother->right->color = BLACK;        //ребенок-черный
                __left_rotate(rb, node->parent);      //левый поворот от родителя
                node = rb->root;
            }
        } else { //правый сын?
            brother = node->parent->left;
            if (brother->color == RED) {
                brother->color = BLACK;
                node->parent->color = RED;
                __right_rotate(rb, node->parent);
                brother = node->parent->left;
            }
            if (brother->left->color == BLACK && brother->right->color == BLACK) {
                brother->color = RED;
                node = node->parent;
            } else {
                if (brother->left->color == BLACK) {
                    brother->right->color = BLACK;
                    brother->color = RED;
                    __left_rotate(rb, brother);
                    brother = node->parent->left;
                }
                brother->color = node->parent->color;
                node->parent->color = BLACK;
                brother->left->color = BLACK;
                __right_rotate(rb, node->parent);
                node = rb->root;
            }
        }
    }
    node->color = BLACK;
}