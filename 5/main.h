#ifndef LAB5_5_MAIN_H_
#define LAB5_5_MAIN_H_

#include "lib/code_status.h"
#include "lib/graph.h"

Graph_Foo add_node(Graph* graph);
Graph_Foo add_edge(Graph* graph);
Graph_Foo delete_node(Graph* graph);
Graph_Foo delete_edge(Graph* graph);
Graph_Foo change_node(Graph* graph);
Graph_Foo change_edge(Graph* graph);
Graph_Foo bfs(Graph* graph);
Graph_Foo bellman_ford(Graph* graph);
Graph_Foo floyd_warshall(Graph* graph);
Graph_Foo graphViz(Graph* graph);
Graph_Foo import(Graph* graph);

#endif