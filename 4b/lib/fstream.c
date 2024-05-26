#include "fstream.h"

#include <stdio.h>

#include "fstream_lib.h"
#include "numbers.h"
#include "rb_lib.h"
#include "readline.h"

Foo
import_txt(RB* rb, const char* file_name) {
    Foo return_code = OK;
    size_t data = 0;
    size_t* data_ptr = NULL;
    char* key = NULL;
    if (__rb_valid(rb) == BAD_DATA) {
        return BAD_DATA;
    }
    if ((return_code = __rb_dealloc(rb)) != OK) {
        return return_code;
    }
    rb->root = rb->nil;
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return BAD_FILE;
    }
    while ((key = readline(file, "")) != NULL) {
        if (key == NULL) {
            fclose(file);
            return BAD_ALLOC;
        }
        fscanf(file, "%zu", &data);
        data_ptr = gen_number(data);
        if (data_ptr == NULL) {
            free(key);
            fclose(file);
            return OK;
        }
        return_code = rb_insert(rb, key, data_ptr);
        if (return_code != OK && return_code != DUPLICATE) {
            free(data_ptr);
            free(key);
        } else if (return_code == DUPLICATE) {
            free(key);
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
    __rb_postorder(rb, __export_dot);
    file = fopen(file_name, "a");
    fprintf(file, "}\n");
    fclose(file);
    system("dot -Tpng output.dot -o output.png");
    system("rm output.dot");
    return OK;
}

void
__export_dot(RB* rb, Node* node) {
    const char* file_name = "output.dot";
    static FILE* file = NULL;
    if (file == NULL) {
        file = fopen(file_name, "a");
    }
    fprintf(file, "%s [color = %s]\n", (char*)node->key, colors[node->color]);
    if (node->left != rb->nil) {
        fprintf(file, "%s -- %s\n", (char*)node->key, (char*)node->left->key);
    }
    if (node->right != rb->nil) {
        fprintf(file, "%s -- %s\n", (char*)node->key, (char*)node->right->key);
    }
    if (node == rb->root) {
        fclose(file);
        file = NULL;
    }
}