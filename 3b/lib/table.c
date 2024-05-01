#include "table.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hash.h"
#include "info_lib.h"
#include "item_lib.h"
#include "prime.h"
#include "table_lib.h"

Foo
table_init(Table** table, size_t capacity, Info* info) {
    if (table == NULL || __info_valid(info) == BAD_DATA) {
        return BAD_DATA;
    }
    *table = (Table*)malloc(sizeof(Table));
    if (*table == NULL) {
        return BAD_ALLOC;
    }
    (*table)->items = (Item*)calloc(capacity, sizeof(Item));
    if ((*table)->items == NULL) {
        free(*table);
        return BAD_ALLOC;
    }
    (*table)->capacity = capacity;
    (*table)->info = info;
    (*table)->size = 0;
    return OK;
}

Foo
__table_valid(Table* table) {
    if (table == NULL || __info_valid(table->info) == BAD_DATA || __item_valid(table->items) == BAD_DATA) {
        return BAD_DATA;
    }
    return OK;
}

Foo
table_insert(Table** table, void* key, void* data) {
    if (__table_valid(*table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t ind = 0;
    size_t size = (*table)->size, capacity = (*table)->capacity;
    size_t hash_num = hash(key, (*table)->info->key_size, capacity);
    Item* items = (*table)->items;
    fptr_compare compare = (*table)->info->compare;
    if ((*table)->capacity == (*table)->size) {
        return OVERFLOW;
    }
    do {
        hash_num = (hash_num + ind) % capacity;
        if (items[hash_num].busy == 0) {
            break;
        } else if (compare((items[hash_num]).key, key) == 0) {
            return BAD_KEY;
        } else {
            ++ind;
        }
    } while (ind <= size);
    if (__item_fill(key, data, items + hash_num) == BAD_DATA) {
        return BAD_DATA;
    }
    Foo call_back = OK;
    if (++((*table)->size) == capacity) {
        if ((call_back = __table_expand(table)) != OK) {
            return call_back;
        }
    }
    return OK;
}

Foo
table_remove(Table* table, void* key) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t pos = 0;
    if (__table_search(table, key, &pos) == OK) {
        __item_dealloc(table->items + pos, table->info);
        (table->items)[pos].busy = 0;
        return OK;
    }
    return NOT_FOUNDED;
}

Foo
table_search(Table* table, void* key, Item** result) {
    if (__table_valid(table) == BAD_DATA || key == NULL) {
        return BAD_DATA;
    }
    size_t pos = 0;
    Foo call_back = OK;
    if ((call_back = __table_search(table, key, &pos)) != OK) {
        return call_back;
    }
    *result = (Item*)malloc(sizeof(Item));
    if (*result == NULL) {
        return BAD_ALLOC;
    }
    return __item_copy(*result, table->items + pos, table->info);
}

Foo
__table_search(Table* table, void* key, size_t* pos) {
    size_t ind = 0;
    size_t size = table->size, capacity = table->capacity;
    size_t hash_num = hash(key, table->info->key_size, capacity);
    Item* items = table->items;
    fptr_compare compare = table->info->compare;
    char flag = 0;
    do {
        hash_num = (hash_num + ind) % capacity;
        if (compare((items[hash_num]).key, key) == 0 && items[hash_num].busy == 1) {
            flag = 1;
            break;
        } else {
            ++ind;
        }
    } while (ind <= size && items[hash_num].busy == 1);
    if (flag) {
        *pos = hash_num;
        return OK;
    }
    return NOT_FOUNDED;
}

void
table_print(Table* table) {
    size_t capacity = table->capacity;
    Item* items = table->items;
    if (table->size) {
        printf("ind\tbusy\tkey\tdata\n");
    }
    for (size_t i = 0; i < capacity; ++i) {
        printf("%zu\t%d\t", i, items[i].busy);
        item_print(items + i, table->info);
    }
}

void
table_dealloc(Table* table) {
    Item* items = table->items;
    Info* info = table->info;
    size_t capacity = table->capacity;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            __item_dealloc(items + i, info);
        }
    }
    free(items);
    free(table);
}

Foo
__table_expand(Table** table) {
    Table* tmp = NULL;
    Foo call_back = OK;
    size_t capacity = (*table)->capacity;
    if ((call_back = table_init(&tmp, next_prime((capacity * 2)), (*table)->info)) != OK) {
        return call_back;
    }
    Item* items = (*table)->items;
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            table_insert(&tmp, items[i].key, items[i].data);
        }
    }
    free(items);
    free(*table);
    *table = tmp;
    return OK;
}

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

Foo
table_import(Table** table, Info* info, const char* file_name) {
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
        return call_back;
    }
    for (size_t i = 0; i < size; ++i) {
        key = malloc(key_size);
        data = malloc(data_size);
        if (key == NULL || data == NULL) {
            free(key);
            free(data);
            table_dealloc(*table);
            return BAD_ALLOC;
        }
        fread(key, key_size, 1, file);
        fread(data, data_size, 1, file);
        if ((call_back = table_insert(table, key, data)) != OK) {
            free(key);
            free(data);
            table_dealloc(*table);
            return call_back;
        }
    }
    return OK;
}

Foo
table_export(Table* table, const char* file_name) {
    FILE* file = NULL;
    char* text = "hash_table";
    if (access(file_name, F_OK) == 0) {
        file = fopen(file_name, "rb");
        if (__valid_file("hash_table", file) != OK) {
            fclose(file);
            return BAD_FILE;
        }
        fclose(file);
    }
    file = fopen(file_name, "wb");
    if (file == NULL) {
        return BAD_FILE;
    }
    size_t capacity = table->capacity, size = table->size;
    Item* items = table->items;
    size_t key_size = table->info->key_size;
    size_t data_size = table->info->data_size;
    fwrite(text, strlen(text) + 1, 1, file);
    fwrite(&capacity, sizeof(size_t), 1, file);
    fwrite(&size, sizeof(size_t), 1, file);
    for (size_t i = 0; i < capacity; ++i) {
        if (items[i].busy) {
            fwrite(items[i].key, key_size, 1, file);
            fwrite(items[i].data, data_size, 1, file);
        }
    }
    fclose(file);
    return OK;
}