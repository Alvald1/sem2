#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/numbers.h"
#include "lib/readline.h"

#define PROMPT                                                                                                         \
    "\n(i) - insert\n(r) - remove\n(s) - search\n(2) - print_2D\n(d) - print_desc\n(p) - "                             \
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
                if (file(rb) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'g': graphviz(rb); break;
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
    size_t data = 0;
    char* key = NULL;
    if ((key = readline(stdin, "Key: ")) == NULL || read_num(&data, "Data: ") == EOF) {
        rb_dealloc(rb);
        return _EOF;
    }
    size_t* data_ptr = gen_number(data);
    Foo return_code = OK;
    if (data_ptr == NULL) {
        free(key);
        return_code = BAD_ALLOC;
    } else if ((return_code = rb_insert(rb, key, data_ptr)) != OK && return_code != DUPLICATE) {
        free(data_ptr);
        free(key);
    } else if (return_code == DUPLICATE) {
        free(key);
    }
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
_delete(RB* rb) {
    char* key = NULL;
    size_t release = 0;
    if ((key = readline(stdin, "Key: ")) == NULL) {
        rb_dealloc(rb);
        return _EOF;
    }
    if ((read_num(&release, "Release: ")) == EOF) {
        free(key);
        rb_dealloc(rb);
    }
    Foo return_code = rb_delete(rb, key, release);
    free(key);
    fprintf(stderr, "%s", errors[return_code]);
    return OK;
}

Foo
search(RB* rb) {
    Node* node = NULL;
    char* key = NULL;
    if ((key = readline(stdin, "Key: ")) == NULL) {
        rb_dealloc(rb);
        return _EOF;
    }
    Foo return_code = rb_search(rb, key, &node);
    free(key);
    fprintf(stderr, "%s", errors[return_code]);
    printf("\n");
    if (return_code == OK) {
        printf("key\tdata\n");
        node_print(node, rb);
    }
    return OK;
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
    printf("%zu", *((size_t*)data));
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
