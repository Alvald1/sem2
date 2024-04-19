#include "table_iterator.h"

#include <stdlib.h>

#include "table_iterator_lib.h"
#include "table_lib.h"

Foo
__iterator_valid(Iterator* iterator) {
    if (iterator == NULL) {
        return BAD_DATA;
    }
    return OK;
}

Foo
__iterator_init(Iterator** iterator, Info* info) {
    *iterator = (Iterator*)malloc(sizeof(Iterator));
    if (*iterator == NULL) {
        return BAD_ALLOC;
    }
    (*iterator)->info = info;
    (*iterator)->item = NULL;
    return OK;
}

Foo
iterator_first(Table* table, Iterator** iterator) {
    if (__table_valid(table) == BAD_DATA || iterator == NULL || __iterator_valid(*iterator) == BAD_DATA) {
        return BAD_DATA;
    }
    if (__table_size(table) == 0) {
        return EMPTY;
    }
    if (__iterator_init(iterator, table->info) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    (*iterator)->item = __table_item(table, 0);
    return OK;
}