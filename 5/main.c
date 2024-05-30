#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/graph.h"

int
main() {
    Graph* graph = NULL;
    graph_init(&graph);
    graph_add_node(graph, strdup("a"));
    graph_add_node(graph, strdup("b"));
    graph_add_node(graph, strdup("c"));
    graph_add_node(graph, strdup("d"));
    graph_add_node(graph, strdup("e"));
    graph_add_edge(graph, strdup("a"), strdup("b"), 5);
    graph_add_edge(graph, strdup("a"), strdup("c"), 5);
    graph_add_edge(graph, strdup("b"), strdup("d"), 5);
    graph_add_edge(graph, strdup("b"), strdup("e"), 5);
    graph_add_edge(graph, strdup("e"), strdup("c"), 5);
    graph_add_edge(graph, strdup("d"), strdup("a"), 5);
    graph_delete_node(graph, "a");
    graph_graphViz(graph);
    graph_dealloc(graph);
    return 0;
}