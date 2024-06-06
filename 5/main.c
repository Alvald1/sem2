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

    graph_add_edge(graph, ("a"), ("b"), 1);
    graph_add_edge(graph, ("a"), ("c"), 2);
    graph_add_edge(graph, ("b"), ("d"), 3);
    graph_add_edge(graph, ("b"), ("e"), 4);
    graph_add_edge(graph, ("e"), ("c"), 5);
    graph_add_edge(graph, ("d"), ("a"), 6);
    graph_delete_node(graph, "a");
    graph_delete_edge(graph, "b", "d");
    graph_add_edge(graph, ("b"), ("b"), 7);
    graph_add_edge(graph, ("b"), ("c"), 8);
    graph_add_edge(graph, ("b"), ("d"), 9);
    graph_add_edge(graph, ("c"), ("e"), 10);
    graph_add_edge(graph, ("d"), ("e"), -10);
    graph_change_edge(graph, ("d"), ("e"), 10);
    graph_change_node(graph, "b", strdup("a"));
    graph_graphViz(graph);
    graph_dealloc(graph);
    return 0;
}