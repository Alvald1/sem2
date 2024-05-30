#include "graph.h"

#include <stdlib.h>

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
create_node(void* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = NULL;
    return node;
}

Graph_Foo
graph_add_node(Graph* graph, void* data) {
    if (table_insert(graph->table, data, NULL) == HASH_BAD_KEY) {
        return GRAPH_DUPLICATE;
    }
    return GRAPH_OK;
}

Graph_Foo
graph_add_edge(Graph* graph, void* data_first, void* data_second, int weight) {
    if (-10 > weight || weight > 10) {
        return GRAPH_BAD_WEIGHT;
    }
    size_t pos, tmp;
    if (__table_search(graph->table, data_first, &pos) != HASH_OK
        || __table_search(graph->table, data_second, &tmp) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    Node* node = create_node(data_second);
    if (node == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    node->next = (graph->table->items)[pos].data;
    (graph->table->items)[pos].data = node;
    return GRAPH_OK;
}