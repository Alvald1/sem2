#ifndef LAB3B_3B_LIB_CODE_STATUS_H_
#define LAB3B_3B_LIB_CODE_STATUS_H_

typedef enum _hash_foo {
    HASH_EOF = -1,
    HASH_OK,
    HASH_BAD_DATA,
    HASH_BAD_ALLOC,
    HASH_NOT_FOUND,
    HASH_BAD_KEY,
    HASH_BAD_FILE,
    HASH_EMPTY
} Hash_Foo;

typedef enum _hash_status { HASH_NIL, HASH_DELETED, HASH_BUSY } Hash_Status;

extern const char* hash_errors[];
extern const char* hash_statuses[];

#endif