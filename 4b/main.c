#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    RB* rb = NULL;
    Info* info = NULL;
    char status = 0;
    info_init(&info, compare, key_print, data_print, dealloc, dealloc);
    rb_init(&rb, info);
    printf(PROMPT);
    while (scanf("%c", &status) != EOF) {
        switch (status) {
            case 'i':
                if (insert(rb) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'r':
                if (_delete(rb) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 's':
                if (search(rb) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'f':
                scanf("%*c");
                if (file(rb) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'g': graphviz(rb); break;
            case 'm': max(rb); break;
            case '2': print_2D(rb); break;
            case 'p': print_postorder(rb); break;
            case 'd': print_desc(rb); break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf(PROMPT);
    }
    rb_dealloc(rb);
    info_dealloc(info);
    return 0;
}

void
graphviz(RB* rb) {
    Foo return_code = export_dot(rb);
    fprintf(stderr, "%s", errors[return_code]);
}

Foo
file(RB* rb) {
    Foo return_code = OK;
    char* file_name = readline(stdin, "File name: ");
    if (file_name == NULL) {
        rb_dealloc(rb);
        return _EOF;
    }
    return_code = import_txt(rb, file_name);
    free(file_name);
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
insert(RB* rb) {
    size_t key = 0;
    char* data = NULL;
    if (read_num(&key, "Key: ") == EOF || (data = readline(stdin, "Data: ")) == NULL) {
        rb_dealloc(rb);
        return _EOF;
    }
    size_t* key_ptr = gen_number(key);
    Foo return_code = OK;
    void* tmp = NULL;
    if (key_ptr == NULL) {
        free(data);
        return_code = BAD_ALLOC;
    } else if ((return_code = rb_insert(rb, key_ptr, data, &tmp)) != OK && return_code != DUPLICATE) {
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
_delete(RB* rb) {
    size_t num;
    if (read_num(&num, "Key: ") == EOF) {
        rb_dealloc(rb);
        return _EOF;
    }
    Foo return_code = rb_delete(rb, &num);
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
search(RB* rb) {
    Node* node = NULL;
    size_t key = 0;
    if (read_num(&key, "Key: ") == EOF) {
        rb_dealloc(rb);
        return _EOF;
    }
    Foo return_code = rb_search(rb, &key, &node);
    fprintf(stderr, "%s", errors[return_code]);
    printf("\n");
    if (return_code == OK) {
        printf("key\tdata\n");
        node_print(node, rb);
    }
    return OK;
}

void
max(RB* rb) {
    Node* node = rb_maximum(rb);
    if (node != NULL) {
        printf("key\tdata\n");
        node_print(node, rb);
    }
    fprintf(stderr, "%s", errors[OK]);
}

void
print_desc(RB* rb) {
    rb_print_desc(rb);
    fprintf(stderr, "%s", errors[OK]);
}

void
print_2D(RB* rb) {
    rb_print_2D(rb);
    fprintf(stderr, "%s", errors[OK]);
}

void
print_postorder(RB* rb) {
    Foo return_code = rb_print_postorder(rb);
    fprintf(stderr, "%s", errors[return_code]);
}

void
dealloc(void* data) {
    free(data);
}

void
key_print(void* key) {
    printf("%s\t", (char*)key);
}

void
data_print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

Compare
compare(void* left, void* right) {
    int result = strcmp((char*)left, (char*)right);
    if (result > 0) {
        return MORE;
    } else if (result == 0) {
        return EQUAL;
    } else {
        return LESS;
    }
}
