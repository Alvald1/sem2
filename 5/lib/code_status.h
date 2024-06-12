#ifndef LAB5_5_LIB_CODE_STATUS_H_
#define LAB5_5_LIB_CODE_STATUS_H_

typedef enum _graph_foo {
    GRAPH_EOF = -1,
    GRAPH_OK,
    GRAPH_BAD_ALLOC,
    GRAPH_DUPLICATE,
    GRAPH_BAD_WEIGHT,
    GRAPH_BAD_DATA,
    GRAPH_BAD_FILE,
    GRAPH_NEGATIVE_CYCLE
} Graph_Foo;

extern const char* graph_errors[];

#endif