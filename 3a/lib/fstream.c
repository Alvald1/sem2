#include "fstream.h"

#include <stdio.h>
#include <stdlib.h>

#include "info.h"

Foo
read_from_file(char* f_name, Table** table, Info* info) {
    if (f_name == NULL) {
        return BAD_NAME;
    }
    FILE* file = fopen(f_name, "r");
    if (file == NULL) {
        return BAD_FILE;
    }
    size_t capacity = 0;
    if (fscanf(file, "%zu;", &capacity) != 1) {
        fclose(file);
        return BAD_FILE;
    }
    table_dealloc(*table);
    switch (table_init(table, capacity, info)) {
        case BAD_ALLOC: fclose(file); return BAD_ALLOC;
        case BAD_DATA: fclose(file); return BAD_DATA;
        default: break;
    }
    size_t *key = NULL, *data = NULL;
    for (size_t i = 0; i < capacity; ++i) {
        key = malloc(info->key_size);
        data = malloc(info->data_size);
        if (key == NULL || data == NULL) {
            free(key);
            free(data);
            table_dealloc(*table);
            fclose(file);
            return BAD_ALLOC;
        }
        switch (fscanf(file, "%zu,%zu;", key, data)) {
            case EOF:
                free(key);
                free(data);
                fclose(file);
                return OK;
            case 0:
                free(key);
                free(data);
                fscanf(file, "%*[^;]");
                fscanf(file, "%*c");
                break;
            default: table_insert(*table, key, data);
        }
    }
    fclose(file);
    return OK;
}