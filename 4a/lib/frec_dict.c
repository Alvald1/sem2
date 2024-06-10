#include "frec_dict.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "info.h"
#include "info_lib.h"
#include "numbers.h"
#include "tree_lib.h"

char*
__convert_to_buty(void* data, size_t size) {
    char* result = (char*)malloc(size);
    if (result == NULL) {
        return NULL;
    }
    memcpy(result, data, size);
    return result;
}

size_t
__convert_to_num(void* data) {
    size_t num;
    memcpy(&num, data, sizeof(size_t));
    return num;
}

void
__node_print(Node* node, Tree* tree) {
    const char* file_name = "res";
    static FILE* file = NULL;
    if (file == NULL) {
        file = fopen(file_name, "a");
    }
    fprintf(file, "%zu\t", *(size_t*)(node->key));
    size_t num = __convert_to_num(node->data);
    fprintf(file, "%zu\n", num);
    if (node == tree->root) {
        fclose(file);
        file = NULL;
    }
}

Foo
__print(Tree* tree) {
    return __tree_postorder(tree, __node_print);
}

Foo
frec_dict(Tree* tree, const char* file_name) {
    FILE* file = fopen(file_name, "rb");
    if (file == NULL) {
        return BAD_FILE;
    }
    fseek(file, 0, SEEK_END);
    size_t length = ftell(file) / sizeof(size_t);
    fseek(file, 0, SEEK_SET);
    size_t buf[BUFSIZ / sizeof(size_t)];
    size_t cnt = 0, counter = 0;
    Node* node = NULL;
    void *tmp = NULL, *tmp2 = NULL;
    size_t number = 0;
    while ((cnt = fread(buf, sizeof(size_t), BUFSIZ / sizeof(size_t), file))) {
        counter += cnt;
        printf("%f\n", (float)counter / length * 100);
        for (size_t i = 0; i < cnt; ++i) {
            switch (tree_search(tree, (tmp2 = gen_number(buf[i])), &node)) {
                free(tmp2);
                case OK:
                    number = __convert_to_num(node->data) + 1;
                    tree_insert(tree, (tmp2 = gen_number(buf[i])), __convert_to_buty(&number, sizeof(size_t)), &tmp);
                    free(tmp);
                    free(tmp2);
                    break;
                case NOT_FOUND:
                    number = 1;
                    tree_insert(tree, gen_number(buf[i]), __convert_to_buty(&number, sizeof(size_t)), &tmp);
                    break;
                default: return BAD_DATA;
            }
        }
    }
    __print(tree);
    return OK;
}