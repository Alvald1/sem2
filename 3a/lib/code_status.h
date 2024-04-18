#ifndef __CODE_STATUS__
#define __CODE_STATUS__

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
    EMPTY
} Foo;

extern const char* errors_my[];

#endif