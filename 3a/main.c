#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>

#include "general.h"
#include "lib/code_status.h"
#include "lib/fstream.h"
#include "lib/info.h"
#include "lib/item.h"

int
main() {
    Table* table = NULL;
    Info info;
    info.compare = cmp;
    info.data_print = print_item;
    info.key_print = print_item;
    info.data_dealloc = dealloc;
    info.key_dealloc = dealloc;
    info.key_size = sizeof(size_t);
    info.data_size = sizeof(size_t);
    char status = 0;
    table_init(&table, 10, &info);
    printf("(i) - insert\n(r) - remove\n(s) - search\n(p) - print\n(f) - file\n(t) - task\n");
    while (scanf("%c", &status) != EOF) {
        switch (status) {
            case 'i':
                if (insert(table) == EOF) {
                    return 0;
                }
                break;
            case 'r':
                if (rem(table, NULL) == EOF) {
                    return 0;
                }
                break;
            case 's':
                if (search(table, &info) == EOF) {
                    return 0;
                }
                break;
            case 'p': print(table); break;
            case 'f':
                if (file(&table, &info) == EOF) {
                    return 0;
                }
                break;
            case 't':
                if (task(table) == EOF) {
                    return 0;
                }
                break;
            default: printf("Incorrect input_2\n"); break;
        }
        scanf("%*[^\n]");
        scanf("%*c");
        printf("(i) - insert\n(r) - remove\n(s) - search\n(p) - print\n(f) - file\n(t) - task\n");
    }
    table_dealloc(table);
    return 0;
}

Foo
insert(Table* table) {
    size_t* key_ptr = malloc(sizeof(size_t));
    size_t* data = malloc(sizeof(size_t));
    if (key_ptr == NULL || data == NULL) {
        free(key_ptr);
        free(data);
        fprintf(stderr, "%s", errors[BAD_ALLOC]);
        return BAD_ALLOC;
    }
    if (read_num(key_ptr, "Key: ") == EOF || read_num(data, "Data: ") == EOF) {
        free(key_ptr);
        free(data);
        table_dealloc(table);
        return EOF;
    }
    Foo call_back = table_insert(table, key_ptr, data);
    if (call_back == OVERFLOW) {
        free(key_ptr);
        free(data);
    }
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
rem(Table* table, Info* _) {
    size_t num;
    (void)_;
    if (read_num(&num, "Key: ") == EOF) {
        table_dealloc(table);
        return EOF;
    }
    Foo call_back = table_remove(table, &num);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
search(Table* table, Info* info) {
    Item* item = NULL;
    size_t* key_ptr = malloc(sizeof(size_t));
    if (key_ptr == NULL) {
        fprintf(stderr, "%s", errors[BAD_ALLOC]);
        return BAD_ALLOC;
    }
    if (read_num(key_ptr, "Key: ") == EOF) {
        free(key_ptr);
        table_dealloc(table);
        return EOF;
    }
    Foo call_back = table_search(table, key_ptr, &item);
    fprintf(stderr, "%s", errors[call_back]);
    printf("\n");
    if (item) {
        item_print(info, item);
        item_dealloc(info, item);
    } else {
        free(key_ptr);
    }
    return OK;
}

void
print(Table* table) {
    Foo call_back = table_print(table);
    fprintf(stderr, "%s", errors[call_back]);
}

Foo
file(Table** table, Info* info) {
    char* name = readline("File name: ");
    Foo call_back;
    if ((call_back = read_from_file(name, table, info)) == BAD_NAME) {
        table_dealloc(*table);
        return EOF;
    }
    free(name);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}

Foo
task(Table* table) {
    size_t num, num2;
    if (read_num(&num, "Left: ") == EOF || read_num(&num2, "Right: ") == EOF) {
        table_dealloc(table);
        return EOF;
    }
    Foo call_back = table_remove_by_range(table, &num, &num2);
    fprintf(stderr, "%s", errors[call_back]);
    return OK;
}
