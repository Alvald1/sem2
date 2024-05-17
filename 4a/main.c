#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "lib/info.h"
#include "lib/tree.h"

int
main() {
    Tree* tree = NULL;
    Info* info = NULL;
    void *tmp = NULL, *tmp2 = NULL;
    info_init(&info, compare, print, print, dealloc, dealloc, sizeof(size_t));
    tree_init(&tree, info);
    tree_insert(tree, gen_number(5), gen_number(5), &tmp);
    tree_insert(tree, gen_number(3), gen_number(3), &tmp);
    tree_insert(tree, gen_number(7), gen_number(7), &tmp);
    tree_insert(tree, gen_number(4), gen_number(4), &tmp);
    tree_insert(tree, gen_number(11), gen_number(11), &tmp);
    tree_insert(tree, gen_number(13), gen_number(13), &tmp);
    tree_insert(tree, gen_number(19), gen_number(19), &tmp);
    tree_insert(tree, (tmp2 = gen_number(19)), gen_number(120), &tmp);
    free(tmp);
    free(tmp2);
    tree_insert(tree, gen_number(8), gen_number(8), &tmp);
    tree_insert(tree, gen_number(2), gen_number(2), &tmp);
    tree_print(tree);
    printf("\n\n");
    tree_delete(tree, (tmp = gen_number(5)));
    free(tmp);
    tree_delete(tree, (tmp = gen_number(11)));
    free(tmp);
    tree_delete(tree, (tmp = gen_number(7)));
    free(tmp);
    tree_delete(tree, (tmp = gen_number(4)));
    free(tmp);
    tree_insert(tree, gen_number(5), gen_number(5), &tmp);
    tree_print(tree);
    tree_dealloc(tree);
    info_dealloc(info);
    return 0;
}

void
dealloc(void* data) {
    free(data);
}

void
print(void* data) {
    printf("%zu ", *((size_t*)data));
}

Compare
compare(void* left, void* right) {
    if (*((size_t*)left) > *((size_t*)right)) {
        return MORE;
    } else if (*((size_t*)left) == *((size_t*)right)) {
        return EQUAL;
    } else {
        return LESS;
    }
}

size_t*
gen_number(size_t value) {
    size_t* number = malloc(sizeof(size_t));
    if (number == NULL) {
        return NULL;
    }
    *number = value;
    return number;
}
