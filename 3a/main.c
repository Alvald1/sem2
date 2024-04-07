#include "main.h"
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include "lib/code_status.h"
#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/item.h"

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
    size_t *key_ptr, *data;
    size_t num, num2;
    Item* item;
    char* name = NULL;
    table_init(&table, 10, &info);
    printf("(a) - insert\n(b) - remove\n(c) - search\n(d) - print\n(e) - import\n(f) - task\n");
    while (scanf("%c", &status) != EOF) {
        switch (status) {
            case 'a':
                key_ptr = malloc(sizeof(size_t));
                data = malloc(sizeof(size_t));
                if (read_num(key_ptr, "Key: ") == EOF || read_num(data, "Data: ") == EOF) {
                    free(key_ptr);
                    free(data);
                    table_dealloc(&table);
                    return 0;
                }
                table_insert(&table, key_ptr, data);
                break;
            case 'b':
                if (read_num(&num, "Key: ") == EOF) {
                    table_dealloc(&table);
                    return 0;
                }
                table_remove(&table, &num);
                break;
            case 'c':
                key_ptr = malloc(sizeof(size_t));
                if (read_num(key_ptr, "Key: ") == EOF) {
                    free(key_ptr);
                    table_dealloc(&table);
                    return 0;
                }
                table_search(&table, key_ptr, &item);
                printf("\n");
                if (item) {
                    item_print(&info, item);
                    item_dealloc(&info, item);
                } else {
                    free(key_ptr);
                }
                item = NULL;
                break;
            case 'd': table_print(&table); break;
            case 'e':
                name = readline("File name: ");
                if (read_from_file(name, &table, &info) == BAD_NAME) {
                    table_dealloc(&table);
                    return 0;
                }
                free(name);
                break;
            case 'f':
                if (read_num(&num, "Left: ") == EOF || read_num(&num2, "Right: ") == EOF) {
                    table_dealloc(&table);
                    return 0;
                }
                task(&table, num, num2);
                break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf("(a) - insert\n(b) - remove\n(c) - search\n(d) - print\n(e) - import\n(f) - task\n");
    }
    table_dealloc(&table);
    return 0;
}

void
task(Table* table, size_t left, size_t right) {
    for (; left <= right; ++left) {
        table_remove(table, &left);
    }
}

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
read_num(size_t* num, const char* prompt) {
    printf("%s", prompt);
    return get_number("%zu", num);
}