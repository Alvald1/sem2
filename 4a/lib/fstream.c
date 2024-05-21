#include "fstream.h"

#include <stdio.h>

#include "fstream_lib.h"
#include "numbers.h"
#include "readline.h"
#include "tree_lib.h"

Foo
import_txt(Tree* tree, const char* file_name) {
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
        fscanf(file, "%*c");
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

Foo
export_dot(Tree* tree) {
    if (__tree_valid(tree) == BAD_DATA) {
        return BAD_DATA;
    }
    const char* file_name = "output.dot";
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return BAD_FILE;
    }
    fprintf(file, "graph BST {\n");
    fclose(file);
    Foo return_code = __tree_postorder(tree, __export_dot);
    if (return_code != OK) {
        return return_code;
    }
    file = fopen(file_name, "a");
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng output.dot -o output.png");
    system("rm output.dot");
    return OK;
}

void
__export_dot(Node* node, Tree* tree) {
    const char* file_name = "output.dot";
    static FILE* file = NULL;
    if (file == NULL) {
        file = fopen(file_name, "a");
    }
    fprintf(file, "%zu\n", *((size_t*)node->key));
    if (node->left != NULL) {
        fprintf(file, "%zu -- %zu\n", *((size_t*)node->key), *((size_t*)node->left->key));
    }
    if (node->right != NULL) {
        fprintf(file, "%zu -- %zu\n", *((size_t*)node->key), *((size_t*)node->right->key));
    }
    if (node == tree->root) {
        fclose(file);
        file = NULL;
    }
}