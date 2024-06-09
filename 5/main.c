#include "main.h"

#include <stdio.h>

#include "lib/numbers.h"
#include "lib/readline.h"

#define PROMPT                                                                                                         \
    "\n(a) - add_node\n(A) - add_edge\n(d) - delete_node\n(D) - delete_edge\n(c) - change_node\n"                      \
    "(C) - change_edge\n(b) - bfs\n(B) - bellman_ford\n"                                                               \
    "(f) - floyd_warshall\n(g) - graphViz\n(i) - import\n"

int
main() {
    Graph* graph = NULL;
    graph_init(&graph);
    char status = 0;
    printf(PROMPT);
    while (scanf("%c", &status) != EOF) {
        scanf("%*[^\n]");
        scanf("%*c");
        switch (status) {
            case 'a':
                if (add_node(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'A':
                if (add_edge(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'd':
                if (delete_node(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'D':
                if (delete_edge(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'c':
                if (change_node(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'C':
                if (change_edge(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'b':
                if (bfs(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'B':
                if (bellman_ford(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'f':
                if (floyd_warshall(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'i':
                if (import(graph) == GRAPH_EOF) {
                    graph_dealloc(graph);
                    return 0;
                }
                break;
            case 'g': graphViz(graph); break;
            default: printf("Incorrect input_2\n"); break;
        }
        printf(PROMPT);
    }
    graph_dealloc(graph);
    return 0;
}

Graph_Foo
add_node(Graph* graph) {
    char* data = readline(stdin, "Name: ");
    if (data == NULL) {
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_add_node(graph, data);
    if (return_code != GRAPH_OK) {
        free(data);
    }
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
add_edge(Graph* graph) {
    char* first = readline(stdin, "First: ");
    if (first == NULL) {
        return GRAPH_EOF;
    }
    char* second = readline(stdin, "Second: ");
    if (second == NULL) {
        free(first);
        return GRAPH_EOF;
    }
    int weight = 0;
    if (read_num(stdin, &weight, "Weight: ") == EOF) {
        free(first);
        free(second);
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_add_edge(graph, first, second, weight);
    free(first);
    free(second);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
delete_node(Graph* graph) {
    char* data = readline(stdin, "Name: ");
    if (data == NULL) {
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_delete_node(graph, data);
    free(data);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
delete_edge(Graph* graph) {
    char* first = readline(stdin, "First: ");
    if (first == NULL) {
        return GRAPH_EOF;
    }
    char* second = readline(stdin, "Second: ");
    if (second == NULL) {
        free(first);
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_delete_edge(graph, first, second);
    free(first);
    free(second);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
change_node(Graph* graph) {
    char* first = readline(stdin, "Old: ");
    if (first == NULL) {
        return GRAPH_EOF;
    }
    char* second = readline(stdin, "New: ");
    if (second == NULL) {
        free(first);
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_change_node(graph, first, second);
    if (return_code != GRAPH_OK) {
        free(second);
    }
    free(first);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
change_edge(Graph* graph) {
    char* first = readline(stdin, "First: ");
    if (first == NULL) {
        return GRAPH_EOF;
    }
    char* second = readline(stdin, "Second: ");
    if (second == NULL) {
        free(first);
        return GRAPH_EOF;
    }
    int weight = 0;
    if (read_num(stdin, &weight, "Weight: ") == EOF) {
        free(first);
        free(second);
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_change_edge(graph, first, second, weight);
    free(first);
    free(second);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
bfs(Graph* graph) {
    char* data = readline(stdin, "Name: ");
    if (data == NULL) {
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_bfs(graph, data);
    free(data);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
bellman_ford(Graph* graph) {
    char* first = readline(stdin, "First: ");
    if (first == NULL) {
        return GRAPH_EOF;
    }
    char* second = readline(stdin, "Second: ");
    if (second == NULL) {
        free(first);
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_bellman_ford(graph, first, second);
    free(first);
    free(second);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
floyd_warshall(Graph* graph) {
    char* data = readline(stdin, "Name: ");
    if (data == NULL) {
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_floyd_warshall(graph, data);
    fprintf(stderr, "%s", graph_errors[return_code]);
    free(data);
    return GRAPH_OK;
}

Graph_Foo
graphViz(Graph* graph) {
    Graph_Foo return_code = graph_graphViz(graph);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}

Graph_Foo
import(Graph* graph) {
    char* data = readline(stdin, "File name: ");
    if (data == NULL) {
        return GRAPH_EOF;
    }
    Graph_Foo return_code = graph_import(graph, data);
    free(data);
    fprintf(stderr, "%s", graph_errors[return_code]);
    return GRAPH_OK;
}