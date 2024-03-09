#include <stdio.h>
#include <stdlib.h>
#include "code_status.h"
#include "list.h"

int
dealloc_dequeue(Dequeue* dequeue) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* head = get_head(dequeue);
    while (head) {
        Node* tmp = head;
        head = head->next;
        free(tmp);
    }
    free(dequeue);
    return OK;
}

void
dealloc_node(Node* node) {
    free(node);
}

int
print_dequeue(const Dequeue* dequeue, fptr_print_data fptr) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* head = get_head(dequeue);
    if (head == NULL) {
        return EMPTY;
    }
    do {
        (*fptr)(get_data(head));
    } while ((head = head->next));
    printf("\n");
    return OK;
}

int
pop_front(Dequeue* dequeue, void** data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* head = get_head(dequeue);
    if (head == NULL) {
        return EMPTY;
    }
    Node* next = get_next(head);
    if (next == NULL) {
        set_head(dequeue, NULL);
        set_tail(dequeue, NULL);
    } else {
        set_prev(next, NULL);
        set_head(dequeue, next);
    }
    *data = get_data(head);
    dealloc_node(head);
    return OK;
}

int
pop_back(Dequeue* dequeue, void** data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* tail = get_tail(dequeue);
    if (tail == NULL) {
        return EMPTY;
    }
    Node* prev = get_prev(tail);
    if (prev == NULL) {
        set_head(dequeue, NULL);
        set_tail(dequeue, NULL);
    } else {
        set_next(prev, NULL);
        set_tail(dequeue, prev);
    }
    *data = get_data(tail);
    dealloc_node(tail);
    return OK;
}

int
push_front(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* new_node = get_node(data);
    if (new_node == NULL) {
        return BAD_ALLOC;
    } else {
        Node* head = get_head(dequeue);
        if (head == NULL) {
            set_head(dequeue, new_node);
            set_tail(dequeue, new_node);
        } else {
            set_next(new_node, head);
            set_prev(head, new_node);
            set_head(dequeue, new_node);
        }
    }
    return OK;
}

int
push_back(Dequeue* dequeue, void* data) {
    if (dequeue == NULL) {
        return BAD_DEQUEUE;
    }
    Node* new_node = get_node(data);
    if (new_node == NULL) {
        return BAD_ALLOC;
    } else {
        Node* tail = get_tail(dequeue);
        if (tail == NULL) {
            set_head(dequeue, new_node);
            set_tail(dequeue, new_node);
        } else {
            set_prev(new_node, tail);
            set_next(tail, new_node);
            set_tail(dequeue, new_node);
        }
    }
    return OK;
}

Node*
get_node(void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return NULL;
    }
    set_data(new_node, data);
    set_next(new_node, NULL);
    set_prev(new_node, NULL);
    return new_node;
}

int
init_dequeue(Dequeue** dequeue, size_t _) {
    (void)_;
    *dequeue = malloc(sizeof(Dequeue));
    if (*dequeue == NULL) {
        return BAD_ALLOC;
    }
    set_head(*dequeue, NULL);
    set_tail(*dequeue, NULL);
    return OK;
}

Node*
get_next(const Node* node) {
    return node->next;
}

Node*
get_prev(const Node* node) {
    return node->prev;
}

void
set_next(Node* node, Node* next) {
    node->next = next;
}

void
set_prev(Node* node, Node* prev) {
    node->prev = prev;
}

void*
get_data(const Node* node) {
    return node->data;
}

void
set_data(Node* node, void* data) {
    node->data = data;
}

Node*
get_head(const Dequeue* dequeue) {
    return dequeue->head;
}

void
set_head(Dequeue* dequeue, Node* head) {
    dequeue->head = head;
}

Node*
get_tail(const Dequeue* dequeue) {
    return dequeue->tail;
}

void
set_tail(Dequeue* dequeue, Node* tail) {
    dequeue->tail = tail;
}