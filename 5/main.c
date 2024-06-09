#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/graph.h"

int
main() {
    Graph* graph = NULL;
    graph_init(&graph);
    graph_import(graph, "test");

    graph_bfs(graph, "d");
    graph_bellman_ford(graph, "a", "d");
    graph_floyd_warshall(graph, "d");
    graph_graphViz(graph);
    graph_dealloc(graph);
    return 0;
}