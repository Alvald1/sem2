#ifndef LAB5_5_LIB_GRAPH_LIB_H_
#define LAB5_5_LIB_GRAPH_LIB_H_

#include <stddef.h>

#include "../hash_table/table.h"

#define INF INT_MAX

typedef struct _node {
    void* data;
    Node* next;
} Node;

struct _graph {
    Table* table;
};

#endif