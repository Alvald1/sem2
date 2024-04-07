#include "main.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"
#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/item.h"
#include "lib/table.h"

void
dealloc(void* data) {
    free(data);
}

void
print(void* data) {
    printf("%zu\t", *((size_t*)data));
}

int
cmp(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return 1;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return 0;
    } else if (*((size_t*)left) < *((size_t*)right)) {
        return -1;
    }
    return OK;
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
read_key(size_t* key) {
    printf("Key: ");
    return get_number("%zu", key);
}

int
read_data(size_t* data) {
    printf("data: ");
    return get_number("%zu", data);
}

int
main() {
    Table table;
    Info info;
    info.compare = cmp;
    info.data_print = print;
    info.key_print = print;
    info.data_dealloc = dealloc;
    info.key_dealloc = dealloc;
    info.key_size = sizeof(size_t);
    info.data_size = sizeof(size_t);
    char status = 0;
    size_t *key, *data;
    Item* item;
    char* name = NULL;
    table_init(&table, 10, &info);
    printf("(a) - insert\n(b) - remove\n(c) - search\n(d) - print\n(e) - import\n");
    while (scanf("%c", &status) != EOF) {
        switch (status) {
            case 'a':
                key = malloc(sizeof(size_t));
                data = malloc(sizeof(size_t));
                if (read_key(key) == EOF || read_data(data) == EOF) {
                    free(key);
                    free(data);
                    table_dealloc(&table);
                    return 0;
                }
                table_insert(&table, key, data);
                break;
            case 'b':
                key = malloc(sizeof(size_t));
                if (read_key(key) == EOF) {
                    free(key);
                    table_dealloc(&table);
                    return 0;
                }
                table_remove(&table, key);
                free(key);
                break;
            case 'c':
                key = malloc(sizeof(size_t));
                if (read_key(key) == EOF) {
                    free(key);
                    table_dealloc(&table);
                    return 0;
                }
                table_search(&table, key, &item);
                printf("\n");
                if (item) {
                    item_print(&info, item);
                    item_dealloc(&info, item);
                }
                break;
            case 'd': table_print(&table); break;
            case 'e':
                name = readline("File name: ");
                read_from_file(name, &table, &info);
                free(name);
                break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf("(a) - insert\n(b) - remove\n(c) - search\n(d) - print\n(e) - import\n");
    }
    table_dealloc(&table);
    return 0;
}