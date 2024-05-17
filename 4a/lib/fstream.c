#include "fstream.h"

#include <stdio.h>

#include "numbers.h"
#include "readline.h"
#include "tree_lib.h"

Foo
import(Tree* tree, const char* file_name) {
    Foo return_code = OK;
    size_t key = 0;
    size_t* key_ptr = NULL;
    void* tmp = NULL;
    char* data = NULL;
    if (__tree_valid(tree) == BAD_DATA) {
        return BAD_DATA;
    }
    if ((return_code = __tree_dealloc(tree)) != OK) {
        return return_code;
    }
    tree->root = NULL;
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return BAD_FILE;
    }
    while (fscanf(file, "%zu", &key) != EOF) {
        key_ptr = gen_number(key);
        if (key_ptr == NULL) {
            fclose(file);
            return BAD_ALLOC;
        }
        data = readline(file, "");
        if (data == NULL) {
            free(key_ptr);
            fclose(file);
            return OK;
        }
        if ((return_code = tree_insert(tree, key_ptr, data, &tmp)) != OK && return_code != DUPLICATE) {
            free(key_ptr);
            free(data);
        } else if (return_code == DUPLICATE) {
            free(key_ptr);
            free(tmp);
        }
        fprintf(stderr, "%s", errors[return_code]);
    }
    fclose(file);
    return OK;
}