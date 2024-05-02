#include "fstream_lib.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "info_lib.h"
#include "item_lib.h"
#include "table_lib.h"

Foo
__valid_file(const char* str, FILE* file) {
    void* tmp = NULL;
    tmp = malloc(strlen(str) + 1);
    if (tmp == NULL) {
        return BAD_ALLOC;
    }
    fread(tmp, strlen(str) + 1, 1, file);
    if (strcmp((char*)tmp, str) != 0) {
        free(tmp);
        return BAD_FILE;
    }
    free(tmp);
    return OK;
}

void
__exit(void* key, void* data, Table* table, FILE* file) {
    free(key);
    free(data);
    table_dealloc(table);
    fclose(file);
}

Foo
table_import(Table** table, Info* info, const char* file_name) {
    if (table == NULL) {
        return BAD_DATA;
    }
    if (*table != NULL) {
        table_dealloc(*table);
    }
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        return BAD_FILE;
    }
    if (__valid_file("hash_table", file) != OK) {
        fclose(file);
        return BAD_FILE;
    }
    size_t key_size = info->key_size, data_size = info->data_size;
    size_t capacity = 0, size = 0;
    fread(&capacity, sizeof(size_t), 1, file);
    fread(&size, sizeof(size_t), 1, file);
    Foo call_back = OK;
    void *key = NULL, *data = NULL;
    if ((call_back = table_init(table, capacity, info)) != OK) {
        fclose(file);
        return call_back;
    }
    for (size_t i = 0; i < size; ++i) {
        key = malloc(key_size);
        data = malloc(data_size);
        if (key == NULL || data == NULL) {
            __exit(key, data, *table, file);
            return BAD_ALLOC;
        }
        fread(key, key_size, 1, file);
        fread(data, data_size, 1, file);
        if ((call_back = table_insert(table, key, data)) != OK) {
            __exit(key, data, *table, file);
            return call_back;
        }
    }
    fclose(file);
    return OK;
}

Foo
table_export(Table* table, const char* file_name) {
    FILE* file = NULL;
    char* text = "hash_table";
    if (access(file_name, F_OK) == 0) {
        file = fopen(file_name, "rb");
        if (__valid_file(text, file) != OK) {
            fclose(file);
            return BAD_FILE;
        }
        fclose(file);
    }
    file = fopen(file_name, "wb");
    if (file == NULL) {
        return BAD_FILE;
    }
    size_t capacity = table->capacity;
    Item* items = table->items;
    size_t key_size = table->info->key_size;
    size_t data_size = table->info->data_size;
    fwrite(text, strlen(text) + 1, 1, file);
    fwrite(&capacity, sizeof(size_t), 1, file);
    fwrite(&(table->size), sizeof(size_t), 1, file);
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            fwrite(items[i].key, key_size, 1, file);
            fwrite(items[i].data, data_size, 1, file);
        }
    }
    fclose(file);
    return OK;
}