#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "lib/code_status.h"
#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/table.h"

int
main() {
    Table* table = NULL;
    Info* info;
    char status = 0;
    info_init(&info, compare, dealloc, dealloc, print, print, sizeof(size_t), sizeof(size_t));
    table_init(&table, 1, info);
    printf("(i) - insert\n(r) - remove\n(s) - search\n(p) - print\n(e) - export\n(f) - import\n");
    while (scanf("%c", &status) != _EOF) {
        switch (status) {
            case 'i':
                if (insert(&table) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'r':
                if (_remove(table) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 's':
                if (search(table, info) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'p': _print(table); break;
            case 'e':
                if (_export(table) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            case 'f':
                if (import(&table, info) == _EOF) {
                    info_dealloc(info);
                    return 0;
                }
                break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf("(i) - insert\n(r) - remove\n(s) - search\n(p) - print\n(e) - export\n(f) - import\n");
    }
    table_dealloc(table);
    info_dealloc(info);
    return 0;
}

Foo
insert(Table** table) {
    size_t key = 0, data = 0;
    if (read_num(&key, "Key: ") == EOF || read_num(&data, "Data: ") == EOF) {
        table_dealloc(*table);
        return _EOF;
    }
    size_t *key_ptr = gen_number(key), *data_ptr = gen_number(data);
    Foo call_back = OK;
    if (key_ptr == NULL || data_ptr == NULL) {
        free(key_ptr);
        free(data_ptr);
        call_back = BAD_ALLOC;
    } else {
        if ((call_back = table_insert(table, key_ptr, data_ptr)) != OK) {
            free(key_ptr);
            free(data_ptr);
        }
    }
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
_remove(Table* table) {
    size_t num;
    if (read_num(&num, "Key: ") == EOF) {
        table_dealloc(table);
        return _EOF;
    }
    Foo call_back = table_remove(table, &num);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
search(Table* table, Info* info) {
    Item* item = NULL;
    size_t key = 0;
    if (read_num(&key, "Key: ") == EOF) {
        table_dealloc(table);
        return _EOF;
    }
    Foo call_back = table_search(table, &key, &item);
    fprintf(stderr, "%s", errors[call_back]);
    printf("\n");
    if (call_back == OK) {
        printf("key\tdata\n");
        item_print(item, info);
        item_dealloc(item, info);
    }
    return OK;
}

Foo
_export(Table* table) {
    char* file_name = readline("File name: ");
    if (file_name == NULL) {
        table_dealloc(table);
        return _EOF;
    }
    Foo call_back = table_export(table, file_name);
    free(file_name);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
import(Table** table, Info* info) {
    char* file_name = readline("File name: ");
    if (file_name == NULL) {
        table_dealloc(*table);
        return _EOF;
    }
    Foo call_back = table_import(table, info, file_name);
    free(file_name);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

void
_print(Table* table) {
    Foo call_back = table_print(table);
    fprintf(stderr, "%s", errors[call_back]);
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
compare(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return 1;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return 0;
    } else {
        return -1;
    }
}

void
print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

void
dealloc(void* data) {
    free(data);
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