#ifndef LAB5_5_LIB_GRAPH_H_
#define LAB5_5_LIB_GRAPH_H_

#include "../hash_table/info.h"
#include "code_status.h"

typedef struct _graph Graph;

Graph_Foo graph_init(Graph** graph);
void graph_dealloc(Graph* graph);
Graph_Foo graph_add_node(Graph* graph, void* data);
Graph_Foo graph_add_edge(Graph* graph, void* data_first, void* data_second, int weight);
Graph_Foo graph_delete_node(Graph* graph, void* data);
Graph_Foo graph_delete_edge(Graph* graph, void* data_first, void* data_second);
Graph_Foo graph_change_node(Graph* graph, void* data, void* data_new);
Graph_Foo graph_change_edge(Graph* graph, void* data_first, void* data_second, int weight);
Graph_Foo graph_bfs(Graph* graph, void* data);
Graph_Foo graph_bellman_ford(Graph* graph, void* data_first, void* data_second);
Graph_Foo graph_floyd_warshall(Graph* graph, void* data_first);
Graph_Foo graph_graphViz(Graph* graph);
Graph_Foo graph_import(Graph* graph, const char* file_name);

#endif