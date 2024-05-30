#ifndef LAB5_5_LIB_GRAPH_LIB_H_
#define LAB5_5_LIB_GRAPH_LIB_H_

#include <stddef.h>

#include "../hash_table/table.h"
#include "graph.h"

#define INF INT_MAX

typedef struct _node {
    void* data;
    int weight;
    struct _node* next;
} Node;

typedef struct _back_trace {
    void* data;
    struct _back_trace* next;
} Back_Trace;

typedef struct _node_info {
    Back_Trace* back_trace;
    Node* node;
} Node_Info;

struct _graph {
    Table* table;
};

Node* __node_create(void* data, int weight);
void __data_dealloc(void* data);
Graph_Foo __add_edge(Item* items, Node* new, size_t first, size_t second, void* data_first);

#endif