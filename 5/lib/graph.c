#include "graph.h"

#include <stdio.h>
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
        || info_init(&hash_info, compare, dealloc, __data_dealloc, key_print, data_print, sizeof(Node))
               == HASH_BAD_ALLOC) {
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
__node_create(void* data, int weight, Node* next) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        return NULL;
    }
    node->data = data;
    node->next = next;
    node->weight = weight;
    return node;
}

Back_Trace*
__back_trace_create(void* data, int weight, Back_Trace* next) {
    Back_Trace* back_trace = (Back_Trace*)malloc(sizeof(Back_Trace));
    if (back_trace == NULL) {
        return NULL;
    }
    back_trace->data = data;
    back_trace->next = next;
    back_trace->weight = weight;
    return back_trace;
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
__add_edge(Item* items, size_t first, size_t second, int weight) {
    Node_Info* node_info = items[first].data;
    Node* node = __node_create(items[second].key, weight, node_info->node);
    if (node == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    node_info->node = node;
    node_info = items[second].data;
    Back_Trace* back_trace = __back_trace_create(items[first].key, weight, node_info->back_trace);
    if (back_trace == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    node_info->back_trace = back_trace;
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
    if (__add_edge(graph->table->items, first, second, weight) != GRAPH_OK) {
        return GRAPH_BAD_ALLOC;
    }
    return GRAPH_OK;
}

Graph_Foo
graph_delete_node(Graph* graph, void* data) {
    size_t current, second;
    if (__table_search(graph->table, data, &current) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    Node_Info* node_info = (graph->table->items)[current].data;
    Node* node = NULL;
    void* previous = NULL;
    fptr_compare compare = graph->table->info->compare;
    Back_Trace* back_trace = node_info->back_trace;
    while (back_trace != NULL) {
        __table_search(graph->table, back_trace->data, &second);
        __node_delete((graph->table->items)[second].data, data, compare);
        previous = back_trace;
        back_trace = back_trace->next;
        free(previous);
    }
    node = node_info->node;
    while (node != NULL) {
        __table_search(graph->table, node->data, &second);
        __back_trace_delete((graph->table->items)[second].data, data, compare);
        previous = node;
        node = node->next;
        free(previous);
    }
    node_info->back_trace = NULL;
    node_info->node = NULL;
    table_remove(graph->table, data);
    return GRAPH_OK;
}

Graph_Foo
graph_graphViz(Graph* graph) {
    const char* file_name = "output.dot";
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return GRAPH_BAD_FILE;
    }
    fprintf(file, "digraph {\n");
    Item* items = graph->table->items;
    size_t capacity = graph->table->capacity;
    Node* node = NULL;
    int weight = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            fprintf(file, "%s\n", (char*)items[i].key);
            node = ((Node_Info*)items[i].data)->node;
            while (node != NULL) {
                weight = node->weight;
                fprintf(file, "%s -> %s [label = %d]\n", (char*)items[i].key, (char*)node->data, weight);
                node = node->next;
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng output.dot -o output.png");
    system("rm output.dot");
    return GRAPH_OK;
}

void
__data_dealloc(void* data) {
    Node_Info* node_info = data;
    Node* node = NULL;
    void* previous = NULL;
    Back_Trace* back_trace = NULL;
    node = node_info->node;
    back_trace = node_info->back_trace;
    while (node != NULL) {
        previous = node;
        node = node->next;
        free(previous);
    }
    while (back_trace != NULL) {
        previous = back_trace;
        back_trace = back_trace->next;
        free(previous);
    }
    free(data);
}

void
graph_dealloc(Graph* graph) {
    void* info = graph->table->info;
    table_dealloc(graph->table);
    info_dealloc(info);
    free(graph);
}

void
__node_delete(Node_Info* node_info, void* data, fptr_compare compare) {
    Node* node = node_info->node;
    Node* previous = NULL;
    while (node != NULL) {
        if ((*compare)(node->data, data) == EQUAL) {
            if (previous == NULL) {
                node_info->node = node->next;
            } else {
                previous->next = node->next;
            }
            free(node);
            break;
        }
        previous = node;
        node = node->next;
    }
}

void
__back_trace_delete(Node_Info* node_info, void* data, fptr_compare compare) {
    Back_Trace* previous = NULL;
    Back_Trace* back_trace = node_info->back_trace;
    while (back_trace != NULL) {
        if ((*compare)(back_trace->data, data) == EQUAL) {
            if (previous == NULL) {
                node_info->back_trace = back_trace->next;
            } else {
                previous->next = back_trace->next;
            }
            free(back_trace);
            break;
        }
        previous = back_trace;
        back_trace = back_trace->next;
    }
}

Graph_Foo
graph_delete_edge(Graph* graph, void* data_first, void* data_second) {
    size_t first, second;
    if (__table_search(graph->table, data_first, &first) != HASH_OK
        || __table_search(graph->table, data_second, &second) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    fptr_compare compare = graph->table->info->compare;
    __node_delete((graph->table->items)[first].data, data_second, compare);
    __back_trace_delete((graph->table->items)[second].data, data_first, compare);
    return GRAPH_OK;
}

Graph_Foo
graph_change_node(Graph* graph, void* data, void* data_new) {
    size_t current;
    if (__table_search(graph->table, data, &current) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    Graph_Foo return_code = GRAPH_OK;
    if ((return_code = graph_add_node(graph, data_new)) != GRAPH_OK) {
        return return_code;
    }
    Node_Info* node_info = (graph->table->items)[current].data;
    Node* node = node_info->node;
    Back_Trace* back_trace = node_info->back_trace;
    while (node != NULL) {
        if ((return_code = graph_add_edge(graph, data_new, node->data, node->weight)) != GRAPH_OK) {
            return return_code;
        }
        node = node->next;
    }
    while (back_trace != NULL) {
        if ((return_code = graph_add_edge(graph, back_trace->data, data_new, back_trace->weight)) != GRAPH_OK) {
            return return_code;
        }
        back_trace = back_trace->next;
    }
    if ((return_code = graph_delete_node(graph, data)) != GRAPH_OK) {
        return return_code;
    }
    return GRAPH_OK;
}