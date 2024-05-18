#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/numbers.h"
#include "lib/readline.h"

#define PROMPT                                                                                                         \
    "\n(i) - insert\n(r) - remove\n(s) - search\n(m) - search max key\n(2) - print_2D\n(d) - print_desc\n(p) - "       \
    "print_postorder\n(f) - "                                                                                          \
    "file\n(g) - graphViz\n"

int
main() {
    Tree* tree = NULL;
    Info* info = NULL;
    char status = 0;
    info_init(&info, compare, key_print, data_print, dealloc, dealloc);
    tree_init(&tree, info);
    printf(PROMPT);
    while (scanf("%c", &status) != EOF) {
        switch (status) {
            case 'i':
                if (insert(tree) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'r':
                if (_delete(tree) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 's':
                if (search(tree) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'f':
                scanf("%*c");
                if (file(tree) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'g': graphviz(tree); break;
            case 'm': max(tree); break;
            case '2': print_2D(tree); break;
            case 'p': print_postorder(tree); break;
            case 'd': print_desc(tree); break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf(PROMPT);
    }
    tree_dealloc(tree);
    info_dealloc(info);
    return 0;
}

void
graphviz(Tree* tree) {
    Foo return_code = export_dot(tree);
    fprintf(stderr, "%s", errors[return_code]);
}

Foo
file(Tree* tree) {
    Foo return_code = OK;
    char* file_name = readline(stdin, "File name: ");
    if (file_name == NULL) {
        return _EOF;
        tree_dealloc(tree);
    }
    return_code = import_txt(tree, file_name);
    free(file_name);
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
insert(Tree* tree) {
    size_t key = 0;
    char* data = NULL;
    if (read_num(&key, "Key: ") == EOF || (data = readline(stdin, "Data: ")) == NULL) {
        tree_dealloc(tree);
        return _EOF;
    }
    size_t* key_ptr = gen_number(key);
    Foo return_code = OK;
    void* tmp = NULL;
    if (key_ptr == NULL) {
        free(data);
        return_code = BAD_ALLOC;
    } else if ((return_code = tree_insert(tree, key_ptr, data, &tmp)) != OK && return_code != DUPLICATE) {
        free(key_ptr);
        free(data);
    } else if (return_code == DUPLICATE) {
        free(key_ptr);
        free(tmp);
    }
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
_delete(Tree* tree) {
    size_t num;
    if (read_num(&num, "Key: ") == EOF) {
        tree_dealloc(tree);
        return _EOF;
    }
    Foo return_code = tree_delete(tree, &num);
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
search(Tree* tree) {
    Node* node = NULL;
    size_t key = 0;
    if (read_num(&key, "Key: ") == EOF) {
        tree_dealloc(tree);
        return _EOF;
    }
    Foo return_code = tree_search(tree, &key, &node);
    fprintf(stderr, "%s", errors[return_code]);
    printf("\n");
    if (return_code == OK) {
        printf("key\tdata\n");
        node_print(node, tree);
    }
    return OK;
}

void
max(Tree* tree) {
    Node* node = tree_maximum(tree);
    if (node != NULL) {
        printf("key\tdata\n");
        node_print(node, tree);
    }
    fprintf(stderr, "%s", errors[OK]);
}

void
print_desc(Tree* tree) {
    tree_print_desc(tree);
    fprintf(stderr, "%s", errors[OK]);
}

void
print_2D(Tree* tree) {
    tree_print_2D(tree);
    fprintf(stderr, "%s", errors[OK]);
}

void
print_postorder(Tree* tree) {
    Foo return_code = tree_print_postorder(tree);
    fprintf(stderr, "%s", errors[return_code]);
}

void
dealloc(void* data) {
    free(data);
}

void
key_print(void* key) {
    printf("%zu\t", *((size_t*)key));
}

void
data_print(void* data) {
    printf("%s\t", (char*)data);
}

Compare
compare(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return MORE;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return EQUAL;
    } else {
        return LESS;
    }
}
