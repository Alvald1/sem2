#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "lib/info.h"

#define PROMPT "(i) - insert\n(r) - remove\n(s) - search\n(d) - print_desc\n(p) - print_postorder\n(f) - file\n"

int
main() {
    Tree* tree = NULL;
    Info* info = NULL;
    char status = 0;
    info_init(&info, compare, print, print, dealloc, dealloc);
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

Foo
insert(Tree* tree) {
    size_t key = 0, data = 0;
    if (read_num(&key, "Key: ") == EOF || read_num(&data, "Data: ") == EOF) {
        tree_dealloc(tree);
        return _EOF;
    }
    size_t *key_ptr = gen_number(key), *data_ptr = gen_number(data);
    Foo return_code = OK;
    void* tmp = NULL;
    if (key_ptr == NULL || data_ptr == NULL) {
        free(key_ptr);
        free(data_ptr);
        return_code = BAD_ALLOC;
    } else if ((return_code = tree_insert(tree, key_ptr, data_ptr, &tmp)) != OK) {
        free(key_ptr);
        free(data_ptr);
    }
    dealloc(tmp);
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
        node_dealloc(node, tree);
    }
    return OK;
}

void
print_desc(Tree* tree) {
    tree_print_desc(tree);
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
print(void* data) {
    printf("%zu ", *((size_t*)data));
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

size_t*
gen_number(size_t value) {
    size_t* number = malloc(sizeof(size_t));
    if (number == NULL) {
        return NULL;
    }
    *number = value;
    return number;
}

int
get_number(const char* format, void* number) {
    int call_back = scanf(format, number);
    while (!call_back) {
        scanf("%*[^\n]");
        printf("Incorrect input\n");
        call_back = scanf(format, number);
    }
    return call_back;
}

int
read_num(size_t* num, const char* prompt) {
    printf("%s", prompt);
    return get_number("%zu", num);
}