#include "graph.h"

#include <stdlib.h>

#include "../hash_table/info_lib.h"
#include "../hash_table/item_lib.h"
#include "../hash_table/table_lib.h"
#include "general.h"
#include "graph_lib.h"

Graph_Foo
graph_init(Graph** graph) {
    Hash_Info* hash_info;
    *graph = (Graph*)malloc(sizeof(Graph));
    if (*graph == NULL
        || info_init(&hash_info, compare, dealloc, dealloc, key_print, data_print, sizeof(Node)) == HASH_BAD_ALLOC) {
        return GRAPH_BAD_ALLOC;
    }
    Table* table = NULL;
    if (table_init(&table, 5, hash_info) == HASH_BAD_ALLOC) {
        return GRAPH_BAD_ALLOC;
    }
    (*graph)->table = table;
    return GRAPH_OK;
}

Node*
__node_create(void* data, int weight) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    node->weight = weight;
    return node;
}

Graph_Foo
graph_add_node(Graph* graph, void* data) {
    Node_Info* node_info = (Node_Info*)calloc(1, sizeof(Node_Info));
    if (node_info == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    switch (table_insert(graph->table, data, node_info)) {
        case HASH_BAD_KEY: free(node_info); return GRAPH_DUPLICATE;
        case HASH_OK: break;
        default: free(node_info); return GRAPH_BAD_DATA;
    }
    return GRAPH_OK;
}

Graph_Foo
__add_edge(Node_Info* node, Node* new, void* data_first) {
    new->next = node->node;
    node->node = new;
    Back_Trace* back_trace = (Back_Trace*)malloc(sizeof(Back_Trace));
    if (back_trace == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    back_trace->data = data_first;
    back_trace->next = node->back_trace;
    node->back_trace = back_trace;
    return GRAPH_OK;
}

Graph_Foo
graph_add_edge(Graph* graph, void* data_first, void* data_second, int weight) {
    if (-10 > weight || weight > 10) {
        return GRAPH_BAD_WEIGHT;
    }
    size_t first, second;
    if (__table_search(graph->table, data_first, &first) != HASH_OK
        || __table_search(graph->table, data_second, &second) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    Node* node = create_node(data_second, weight);
    if (node == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    if (__add_edge((Node_Info*)(graph->table->items)[second].data, node, data_first) != GRAPH_OK) {
        return GRAPH_BAD_ALLOC;
    }
    return GRAPH_OK;
}

Graph_Foo
graph_delete_node(Graph* graph, void* data) {
    size_t first, tmp;
    if (__table_search(graph->table, data, &first) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    Node_Info* node_info = NULL;
    Node *node = NULL, *previous = NULL;
    int flag = 1;
    void* tmp = NULL;
    Back_Trace* back_trace = ((Node_Info*)(graph->table->items)[first].data)->back_trace;
    while (back_trace != NULL) {
        __table_search(graph->table, back_trace->data, &tmp);
        node_info = (Node_Info*)(graph->table->items)[tmp].data;
        node = node_info->node;
        flag = 1;
        while (flag == 1 && node != NULL) {
            if (graph->table->info->compare(data, node->data) == EQUAL) {
                if (previous == NULL) {
                    node_info->node = node->next;
                } else {
                    previous->next = node->next;
                }
                flag = 0;
                free(node);
            }
            previous = node;
            node = node->next;
        }
        tmp = back_trace;
        back_trace = back_trace->next;
        free(tmp);
    }
    node_info = (Node_Info*)(graph->table->items)[first].data;
    node = node_info->node;
    previous = NULL;
    while (node != NULL) {
        previous = node;
        node = node->next;
        free(previous);
    }
    return GRAPH_OK;
}