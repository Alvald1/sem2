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
__add_edge(Item* items, Node* new, size_t first, size_t second, void* data_first) {
    new->next = ((Node_Info*)(items)[first].data)->node;
    ((Node_Info*)(items)[first].data)->node = new;
    Back_Trace* back_trace = (Back_Trace*)malloc(sizeof(Back_Trace));
    if (back_trace == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    back_trace->data = data_first;
    back_trace->next = ((Node_Info*)(items)[second].data)->back_trace;
    ((Node_Info*)(items)[second].data)->back_trace = back_trace;
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
    Node* node = __node_create(data_second, weight);
    if (node == NULL) {
        return GRAPH_BAD_ALLOC;
    }
    if (__add_edge(graph->table->items, node, first, second, data_first) != GRAPH_OK) {
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
    Back_Trace* temp = NULL;
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
                free(node->data);
                free(node);
                break;
            }
            previous = node;
            node = node->next;
        }
        temp = back_trace;
        back_trace = back_trace->next;
        free(temp->data);
        free(temp);
    }
    node_info = (Node_Info*)(graph->table->items)[first].data;
    node = node_info->node;
    previous = NULL;
    while (node != NULL) {
        previous = node;
        free(node->data);
        node = node->next;
        free(previous);
    }
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
graph_dealloc(Graph* graph) {
    Node_Info* node_info = NULL;
    Node *node = NULL, *previous = NULL;
    Item* items = graph->table->items;
    size_t capacity = graph->table->capacity;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            node_info = (Node_Info*)(graph->table->items)[i].data;
            node = node_info->node;
            previous = NULL;
            while (node != NULL) {
                previous = node;
                node = node->next;
                free(previous);
            }
            table_dealloc(graph->table);
        }
    }
}