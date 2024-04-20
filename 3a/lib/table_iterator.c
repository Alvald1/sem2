#include "table_iterator.h"

#include <stddef.h>
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
__iterator_get(Table* table, Iterator** iterator, size_t pos) {
    if (__table_valid(table) == BAD_DATA || iterator == NULL) {
        return BAD_DATA;
    }
    if (__table_size(table) == 0) {
        return EMPTY;
    }
    if (__iterator_init(iterator, table->info) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    (*iterator)->item = __table_item(table, pos);
    return OK;
}

Foo
iterator_front(Table* table, Iterator** iterator) {
    return __iterator_get(table, iterator, 0);
}

Foo
iterator_back(Table* table, Iterator** iterator) {
    return __iterator_get(table, iterator, __table_size(table) - 1);
}

Foo
iterator_compare(Iterator* left, Iterator* right) {
    if (left->info == right->info && left->item == right->item) {
        return OK;
    }
    return DIFFERENT;
}

Foo
iterator_next(Table* table, Iterator* iterator, Iterator** next) {
    if (__table_valid(table) == BAD_DATA || next == NULL || __iterator_valid(iterator) == BAD_DATA) {
        return BAD_DATA;
    }
    size_t pos_res = 0;
    switch (__table_search(table, iterator->item->key, &pos_res)) {
        case BAD_COMP: return BAD_COMP;
        case NOT_FOUND: return BAD_ITER;
        default: break;
    }
    if (__table_size(table) == pos_res + 1) {
        return OVERFLOW;
    }
    if (__iterator_init(next, table->info) == BAD_ALLOC) {
        return BAD_ALLOC;
    }
    (*next)->item = __table_item(table, 0);
    return OK;
}

void
iterator_dealloc(Iterator* iterator) {
    free(iterator);
}

Foo
iterator_insert(Table* table, void* key, void* data, Iterator** iterator) {
    size_t result = 0;
    switch (__table_insert_(table, key, data, &result)) {
        case BAD_DATA: return BAD_DATA;
        case BAD_KEY: return BAD_KEY;
        case BAD_ALLOC: return BAD_ALLOC;
        case OVERFLOW: return OVERFLOW;
        case BAD_COMP: return BAD_COMP;
        default: break;
    }
    return __iterator_get(table, iterator, result);
}
