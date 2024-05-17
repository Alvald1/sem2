#ifndef LAB4A_4A_LIB_CODE_STATUS_H_
#define LAB4A_4A_LIB_CODE_STATUS_H_

typedef enum _foo { _EOF = -1, OK, BAD_DATA, BAD_ALLOC, NOT_FOUND, DUPLICATE } Foo;

extern const char* errors[];

#endif