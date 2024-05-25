#include "fstream.h"

#include <stdio.h>

#include "fstream_lib.h"
#include "numbers.h"
#include "rb_lib.h"
#include "readline.h"

Foo
import_txt(RB* rb, const char* file_name) {
    Foo return_code = OK;
    size_t key = 0;
    size_t* key_ptr = NULL;
    char* data = NULL;
    if (__rb_valid(rb) == BAD_DATA) {
        return BAD_DATA;
    }
    if ((return_code = __rb_dealloc(rb)) != OK) {
        return return_code;
    }
    rb->root = NULL;
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
        return_code = rb_insert(rb, key_ptr, data);
        if (return_code != OK && return_code != DUPLICATE) {
            free(key_ptr);
            free(data);
        } else if (return_code == DUPLICATE) {
            free(key_ptr);
        }
        fprintf(stderr, "%s", errors[return_code]);
    }
    fclose(file);
    return OK;
}

Foo
export_dot(RB* rb) {
    if (__rb_valid(rb) == BAD_DATA) {
        return BAD_DATA;
    }
    const char* file_name = "output.dot";
    FILE* file = fopen(file_name, "w");
    if (file == NULL) {
        return BAD_FILE;
    }
    fprintf(file, "graph BST {\n");
    fclose(file);
    Foo return_code = __rb_postorder(rb, __export_dot);
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
__export_dot(Node* node, RB* rb) {
    const char* file_name = "output.dot";
    static FILE* file = NULL;
    if (file == NULL) {
        file = fopen(file_name, "a");
    }
    fprintf(file, "%zu [color = %s]\n", *((size_t*)node->key), colors[node->color]);
    if (node->left != rb->nil) {
        fprintf(file, "%zu -- %zu\n", *((size_t*)node->key), *((size_t*)node->left->key));
    }
    if (node->right != rb->nil) {
        fprintf(file, "%zu -- %zu\n", *((size_t*)node->key), *((size_t*)node->right->key));
    }
    if (node == rb->root) {
        fclose(file);
        file = NULL;
    }
}