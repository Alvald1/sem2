#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../hash_table/info_lib.h"
#include "../hash_table/item_lib.h"
#include "../hash_table/table_lib.h"
#include "general.h"
#include "graph_lib.h"
#include "queue.h"
#include "readline.h"

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
    Item* items = graph->table->items;
    Node* node = ((Node_Info*)items[first].data)->node;
    fptr_compare compare = graph->table->info->compare;
    while (node != NULL) {
        if ((*compare)(node->data, data_second) == EQUAL) {
            return GRAPH_DUPLICATE;
        }
        node = node->next;
    }
    if (__add_edge(items, first, second, weight) != GRAPH_OK) {
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

Graph_Foo
graph_change_edge(Graph* graph, void* data_first, void* data_second, int weight) {
    if (-10 > weight || weight > 10) {
        return GRAPH_BAD_WEIGHT;
    }
    size_t first = 0, second = 0;
    if (__table_search(graph->table, data_first, &first) != HASH_OK
        || __table_search(graph->table, data_second, &second) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    fptr_compare compare = graph->table->info->compare;
    Item* items = graph->table->items;
    Node_Info* node_info = items[first].data;
    Node* node = node_info->node;
    while (node != NULL) {
        if ((*compare)(node->data, data_second) == EQUAL) {
            node->weight = weight;
            break;
        }
    }
    node_info = items[second].data;
    Back_Trace* back_trace = node_info->back_trace;
    while (back_trace != NULL) {
        if ((*compare)(back_trace->data, data_first) == EQUAL) {
            back_trace->weight = weight;
            break;
        }
    }
    return GRAPH_OK;
}

Graph_Foo
graph_bfs(Graph* graph, void* data) {
    size_t first = 0, current = 0, adj = 0;
    if (__table_search(graph->table, data, &first) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    size_t capacity = graph->table->capacity;
    Color* colors = (Color*)calloc(capacity, sizeof(Color));
    int* distance = (int*)malloc(capacity * sizeof(int));
    size_t* parents = (size_t*)malloc(capacity * sizeof(size_t));
    if (colors == NULL || distance == NULL || parents == NULL) {
        free(colors);
        free(distance);
        free(parents);
        return GRAPH_BAD_ALLOC;
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (i != first) {
            distance[i] = INF;
        }
        parents[i] = capacity;
    }
    colors[first] = GRAY;
    distance[first] = 0;
    Queue* queue = queue_create();
    if (queue == NULL) {
        free(colors);
        free(distance);
        free(parents);
        return GRAPH_BAD_ALLOC;
    }
    Node* node = NULL;
    enQueue(queue, first);
    Item* items = graph->table->items;
    while (queue->front != NULL) {
        current = deQueue(queue);
        node = ((Node_Info*)items[current].data)->node;
        while (node != NULL) {
            __table_search(graph->table, node->data, &adj);
            if (node->weight >= 0 && colors[adj] == WHITE) {
                colors[adj] = GRAY;
                distance[adj] = distance[current] + 1;
                parents[adj] = current;
                enQueue(queue, adj);
            }
            node = node->next;
        }
        colors[current] = BLACK;
    }
    FILE* file = fopen("result_bfs", "w");
    for (size_t i = 0; i < capacity; ++i) {
        if (distance[i] > 1 && distance[i] != INF) {
            printf("%s\n", (char*)items[i].key);
            fprintf(file, "%s\n", (char*)items[i].key);
        }
    }
    fclose(file);
    file = fopen("output.dot", "w");
    if (file == NULL) {
        return GRAPH_BAD_FILE;
    }
    fprintf(file, "digraph {\n");
    int weight = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            if (i == first) {
                fprintf(file, "%s [color = red]\n", (char*)items[i].key);
            } else {
                fprintf(file, "%s [label = \"%s | %d\"]\n", (char*)items[i].key, (char*)items[i].key, distance[i]);
            }
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
    system("dot -Tpng output.dot -o output_bfs.png");
    system("rm output.dot");
    free(colors);
    free(distance);
    free(parents);
    queue_dealloc(queue);
    return GRAPH_OK;
}

void
__relax(int* distance, size_t* parents, size_t first, size_t second, int weight) {
    if (distance[second] > distance[first] + weight) {
        distance[second] = distance[first] + weight;
        parents[second] = first;
    }
}

Graph_Foo
graph_bellman_ford(Graph* graph, void* data_first, void* data_second) {
    size_t first = 0, second = 0, adj = 0;
    if (__table_search(graph->table, data_first, &first) != HASH_OK
        || __table_search(graph->table, data_second, &second) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    size_t capacity = graph->table->capacity;
    size_t* path = (size_t*)malloc(capacity * sizeof(size_t));
    int* distance = (int*)malloc(capacity * sizeof(int));
    size_t* parents = (size_t*)malloc(capacity * sizeof(size_t));
    if (path == NULL || distance == NULL || parents == NULL) {
        free(path);
        free(distance);
        free(parents);
        return GRAPH_BAD_ALLOC;
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (i != first) {
            distance[i] = INF / 2;
        }
        path[i] = capacity;
        parents[i] = capacity;
    }
    distance[first] = 0;
    Node* node = NULL;
    Item* items = graph->table->items;
    for (size_t count = 0; count < capacity - 1; ++count) {
        for (size_t i = 0; i < capacity; ++i) {
            if (items[i].status == HASH_BUSY) {
                node = ((Node_Info*)items[i].data)->node;
                while (node != NULL) {
                    __table_search(graph->table, node->data, &adj);
                    __relax(distance, parents, i, adj, node->weight);
                    node = node->next;
                }
            }
        }
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            node = ((Node_Info*)items[i].data)->node;
            while (node != NULL) {
                __table_search(graph->table, node->data, &adj);
                if (distance[adj] > distance[i] + node->weight) {
                    free(path);
                    free(distance);
                    free(parents);
                    return GRAPH_NEGATIVE_CYCLE;
                }
                node = node->next;
            }
        }
    }
    FILE* file = fopen("result_bellman_ford", "w");
    size_t next = second;
    while (next != capacity && next != first) {
        path[next] = parents[next];
        printf("%s <- ", (char*)items[next].key);
        fprintf(file, "%s\n", (char*)items[next].key);
        next = parents[next];
    }
    if (next == capacity) {
        printf("Path not exist\n");
        fclose(file);
        free(path);
        free(distance);
        free(parents);
        return GRAPH_OK;
    }
    printf("%s\n", (char*)items[next].key);
    fprintf(file, "%s\n", (char*)items[next].key);
    fclose(file);
    file = fopen("output.dot", "w");
    if (file == NULL) {
        free(path);
        free(distance);
        free(parents);
        return GRAPH_BAD_FILE;
    }
    fprintf(file, "digraph {\n");
    int weight = 0;
    size_t tmp = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            if (i == first || i == second) {
                fprintf(file, "%s [color = red] [label = \"%s | %d\"]\n", (char*)items[i].key, (char*)items[i].key,
                        distance[i]);
            } else {
                fprintf(file, "%s [label = \"%s | %d\"]\n", (char*)items[i].key, (char*)items[i].key, distance[i]);
            }
            node = ((Node_Info*)items[i].data)->node;
            while (node != NULL) {
                weight = node->weight;
                __table_search(graph->table, node->data, &tmp);
                if (path[tmp] == i) {
                    fprintf(file, "%s -> %s [color = red] [label = %d]\n", (char*)items[i].key, (char*)node->data,
                            weight);
                } else {
                    fprintf(file, "%s -> %s [label = %d]\n", (char*)items[i].key, (char*)node->data, weight);
                }
                node = node->next;
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng output.dot -o output_bellman_ford.png");
    system("rm output.dot");
    free(distance);
    free(path);
    free(parents);
    return GRAPH_OK;
}

Matrix**
__matrix_init(size_t capacity) {
    Matrix** matrix = (Matrix**)malloc(capacity * sizeof(Matrix*));
    if (matrix == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < capacity; ++i) {
        matrix[i] = (Matrix*)malloc(capacity * sizeof(Matrix));
        if (matrix[i] == NULL) {
            for (; i >= 0; --i) {
                free(matrix[i]);
            }
            free(matrix);
            return NULL;
        }
        for (size_t j = 0; j < capacity; ++j) {
            matrix[i][j].status = NOT_OK;
            matrix[i][j].value = -INF / 2;
        }
    }
    return matrix;
}

Matrix**
__convert_to_matrix(Graph* graph) {
    size_t capacity = graph->table->capacity;
    Matrix** matrix = __matrix_init(capacity);
    if (matrix == NULL) {
        return NULL;
    }
    Item* items = graph->table->items;
    Node* node = NULL;
    size_t ind = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            matrix[i][i].status = OK;
            matrix[i][i].value = 0;
            node = ((Node_Info*)items[i].data)->node;
            while (node != NULL) {
                __table_search(graph->table, node->data, &ind);
                matrix[i][ind].value = node->weight;
                node = node->next;
            }
        }
    }
    return matrix;
}

void
__matrix_dealloc(Matrix** matrix, size_t capacity) {
    for (size_t i = 0; i < capacity; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

Graph_Foo
graph_floyd_warshall(Graph* graph, void* data_first) {
    size_t first = 0, second = 0;
    if (__table_search(graph->table, data_first, &first) != HASH_OK) {
        return GRAPH_BAD_DATA;
    }
    size_t capacity = graph->table->capacity;
    Matrix** matrix = __convert_to_matrix(graph);
    Matrix** parents = __matrix_init(capacity);
    size_t* path = (size_t*)malloc(capacity * sizeof(size_t));
    if (matrix == NULL || parents == NULL || path == NULL) {
        __matrix_dealloc(matrix, capacity);
        __matrix_dealloc(parents, capacity);
        free(path);
        return GRAPH_BAD_ALLOC;
    }
    for (size_t i = 0; i < capacity; ++i) {
        if (matrix[i][i].status == OK) {
            for (size_t j = 0; j < capacity; ++j) {
                if (i != j && matrix[i][j].value > -INF / 2) {
                    parents[i][j].value = i;
                }
            }
        }
        path[i] = capacity;
    }
    for (size_t k = 0; k < capacity; ++k) {
        if (matrix[k][k].status == OK) {
            for (size_t i = 0; i < capacity; ++i) {
                if (matrix[i][i].status == OK) {
                    for (size_t j = 0; j < capacity; ++j) {
                        if (matrix[i][j].value < matrix[i][k].value + matrix[k][j].value) {
                            matrix[i][j].value = matrix[i][k].value + matrix[k][j].value;
                            parents[i][j].value = parents[k][j].value;
                        }
                    }
                }
            }
        }
    }
    Item* items = graph->table->items;
    int flag = 1, max = -INF, ind = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (matrix[i][i].value > 0) {
            printf("Cycle\n");
            flag = 0;
            break;
        }
        if (matrix[i][i].status == OK) {
            if (matrix[first][i].value > max) {
                max = matrix[first][i].value;
                ind = i;
            }
        }
    }
    second = ind;
    size_t next = second;
    FILE* file = NULL;
    if (flag) {
        file = fopen("result_floyd_warshall", "w");
        if (parents[first][next].value == -INF / 2) {
            printf("Path not exist\n");
        } else {
            while (next != first) {
                path[next] = parents[first][next].value;
                printf("%s <- ", (char*)items[next].key);
                fprintf(file, "%s <- ", (char*)items[next].key);
                next = parents[first][next].value;
            }
            printf("%s\n", (char*)items[next].key);
            fprintf(file, "%s <- ", (char*)items[next].key);
        }
        fclose(file);
    }
    file = fopen("output.dot", "w");
    if (file == NULL) {
        free(path);
        __matrix_dealloc(matrix, capacity);
        __matrix_dealloc(parents, capacity);
        return GRAPH_BAD_FILE;
    }
    Node* node = NULL;
    fprintf(file, "digraph {\n");
    int weight = 0;
    size_t tmp = 0;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].status == HASH_BUSY) {
            if (i == first || i == second) {
                fprintf(file, "%s [color = red] [label = \"%s | %d\"]\n", (char*)items[i].key, (char*)items[i].key,
                        matrix[first][i].value);
            } else {
                fprintf(file, "%s [label = \"%s | %d\"]\n", (char*)items[i].key, (char*)items[i].key,
                        matrix[first][i].value);
            }
            node = ((Node_Info*)items[i].data)->node;
            while (node != NULL) {
                weight = node->weight;
                __table_search(graph->table, node->data, &tmp);
                if (path[tmp] == i) {
                    fprintf(file, "%s -> %s [color = red] [label = %d]\n", (char*)items[i].key, (char*)node->data,
                            weight);
                } else {
                    fprintf(file, "%s -> %s [label = %d]\n", (char*)items[i].key, (char*)node->data, weight);
                }
                node = node->next;
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng output.dot -o output_floyd_warshall.png");
    system("rm output.dot");
    free(path);
    __matrix_dealloc(matrix, capacity);
    __matrix_dealloc(parents, capacity);
    return GRAPH_OK;
}

Graph_Foo
graph_import(Graph* graph, const char* file_name) {
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return GRAPH_BAD_FILE;
    }
    size_t size = 0;
    int weight = 0;
    void *data = NULL, *first = NULL, *second = NULL;
    fscanf(file, "%zu", &size);
    while (size--) {
        data = readline(file, "");
        graph_add_node(graph, data);
    }
    while ((first = readline(file, "")) != NULL) {
        second = readline(file, "");
        fscanf(file, "%d", &weight);
        graph_add_edge(graph, first, second, weight);
        free(first);
        free(second);
    }
    fclose(file);
    return GRAPH_OK;
}