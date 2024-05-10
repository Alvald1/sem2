#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "lib/info.h"
#include "lib/tree.h"

int
main() {
    Tree* root = NULL;
    Info* info = NULL;
    info_init(&info, compare, print);
    tree_init(&root, info);
    tree_insert(root, gen_number(5), gen_number(5), (void**)gen_number(5));
    tree_insert(root, gen_number(3), gen_number(3), (void**)gen_number(3));
    tree_insert(root, gen_number(7), gen_number(7), (void**)gen_number(7));
    tree_insert(root, gen_number(4), gen_number(4), (void**)gen_number(4));
    tree_insert(root, gen_number(11), gen_number(11), (void**)gen_number(11));
    tree_insert(root, gen_number(13), gen_number(13), (void**)gen_number(13));
    tree_insert(root, gen_number(19), gen_number(19), (void**)gen_number(19));
    tree_insert(root, gen_number(8), gen_number(8), (void**)gen_number(8));
    tree_insert(root, gen_number(2), gen_number(2), (void**)gen_number(2));
    tree_print(root);
    return 0;
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
