#ifndef LAB3B_3B_LIB_CODE_STATUS_H_
#define LAB3B_3B_LIB_CODE_STATUS_H_

typedef enum _foo { _EOF = -1, OK, BAD_DATA, BAD_ALLOC, OVERFLOW, NOT_FOUNDED, BAD_KEY, BAD_FILE, EMPTY } Foo;

extern const char* errors[];

#endif