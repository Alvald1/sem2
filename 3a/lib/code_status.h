#ifndef LAB3A_3A_LIB_CODE_STATUS_H_
#define LAB3A_3A_LIB_CODE_STATUS_H_

typedef enum _foo {
    EOF = -1,
    OK,
    BAD_ALLOC,
    OVERFLOW,
    BAD_DATA,
    BAD_KEY,
    BAD_COMP,
    NOT_FOUND,
    BAD_FILE,
    BAD_NAME,
    EMPTY,
    BAD_ITER,
    DIFFERENT
} Foo;

extern const char* errors_my[];

#endif