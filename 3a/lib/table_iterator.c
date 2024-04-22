#include "table_iterator.h"

#include <stddef.h>
#include <stdlib.h>

#include "table_lib.h"

Foo
table_iterator_insert(Table* table, void* key, void* data, Iterator** iterator) {
    size_t pos = 0;
    Foo call_back = OK;
    if ((call_back = __table_insert_(table, key, data, &pos)) != OK) {
        return call_back;
    }
    return __iterator_get(table, iterator, pos);
}

Foo
table_iterator_remove(Table* table, Iterator* iterator, Iterator** next) {
    Foo call_back = OK;
    size_t pos = 0;
    if ((call_back = __table_remove(table, iterator->item->key, &pos)) != OK) {
        return call_back;
    }
    if (pos + 1 == __table_size(table)) {
        return OVERFLOW;
    }
    return __iterator_get(table, next, pos + 1);
}

Foo
table_iterator_search(Table* table, void* key, Iterator** iterator) {
    Foo call_back = OK;
    size_t pos = 0;
    if ((call_back = __table_search(table, key, &pos)) != OK) {
        return call_back;
    }
    return __iterator_get(table, iterator, pos);
}