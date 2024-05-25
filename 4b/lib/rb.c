#include "rb.h"

#include <stdio.h>
#include <stdlib.h>

#include "info_lib.h"
#include "rb_lib.h"

void
node_print(Node* node, RB* rb) {
    rb->info->key_print(node->key);
    //rb->info->data_print(node->data);
    printf("\n");
}

void
node_dealloc(Node* node, RB* rb) {
    rb->info->key_dealloc(node->key);
    //rb->info->data_dealloc(node->data);
    free(node);
}

Foo
rb_init(RB** rb, Info* info) {
    if (rb == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *rb = (RB*)malloc(sizeof(RB));
    Node* nil = (Node*)calloc(1, sizeof(Node));
    if (*rb == NULL || nil == NULL) {
        free(*rb);
        free(nil);
        return BAD_ALLOC;
    }
    nil->color = BLACK;
    (*rb)->root = nil;
    (*rb)->nil = nil;
    (*rb)->info = info;
    return OK;
}

Foo
rb_insert(RB* rb, void* key, void* data, void** result) {
    if (__rb_valid(rb) == BAD_DATA || key == NULL || data == NULL || result == NULL) {
        return BAD_DATA;
    }
    Foo return_code = OK;
    Node *node_result = rb->nil, *node = NULL;
    switch (rb_search(rb, key, &node_result)) {
        case OK:
            if (__list_push(&(node_result->list), data) == BAD_ALLOC) {
                return BAD_ALLOC;
            }
            return DUPLICATE;
        case NOT_FOUND:
            if ((return_code = __node_init(rb, &node, key, data)) != OK) {
                return return_code;
            }
            if (node_result == rb->nil) {
                rb->root = node;
            } else if ((*rb->info->compare)(key, node_result->key) == LESS) {
                node_result->left = node;
            } else {
                node_result->right = node;
            }
            node->parent = node_result;
            break;
        default: return BAD_DATA;
    }
    __rb_insert_fixup(rb, node);
    return OK;
}

Foo
rb_delete(RB* rb, void* key) {
    if (__rb_valid(rb) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    Foo return_code = OK;
    Node *successor = NULL, *result = NULL;
    if ((return_code = rb_search(rb, key, &result)) != OK) {
        return return_code;
    }
    if (result->left == NULL) {
        __rb_transplant(rb, result, result->right);
    } else if (result->right == NULL) {
        __rb_transplant(rb, result, result->left);
    } else {
        successor = __node_minimum(result->right);
        if (successor != result->right) {
            __rb_transplant(rb, successor, successor->right);
            successor->right = result->right;
            successor->right->parent = successor;
        }
        __rb_transplant(rb, result, successor);
        successor->left = result->left;
        successor->left->parent = successor;
    }
    node_dealloc(result, rb);
    return OK;
}

Node*
rb_maximum(RB* rb) {
    Node* root = rb->root;
    while (root != NULL && root->right != NULL) {
        root = root->right;
    }
    return root;
}

Foo
rb_search(RB* rb, void* key, Node** result) {
    if (__rb_valid(rb) == BAD_DATA || key == NULL || result == NULL) {
        return BAD_DATA;
    }
    fptr_compare compare = rb->info->compare;
    Compare return_code = EQUAL;
    Node *root = rb->root, *parent = rb->nil;
    while (root != rb->nil && (return_code = (*compare)(key, root->key)) != EQUAL) {
        parent = root;
        if (return_code == LESS) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    if (root != rb->nil) {
        *result = root;
        return OK;
    }
    *result = parent;
    return NOT_FOUND;
}

Foo
rb_dealloc(RB* rb) {
    Foo return_code = __rb_dealloc(rb);
    if (return_code != OK) {
        return return_code;
    }
    free(rb);
    return OK;
}

void
rb_print_2D(RB* rb) {
    __rb_2D(rb, rb->root, 0);
}

Foo
rb_print_postorder(RB* rb) {
    printf("key\tdata\n");
    return __rb_postorder(rb, node_print);
}

void
rb_print_desc(RB* rb) {
    printf("key\tdata\n");
    __rb_desc(rb, rb->root);
}
