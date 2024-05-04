#ifndef LAB3B_3B_LIB_CODE_STATUS_H_
#define LAB3B_3B_LIB_CODE_STATUS_H_

typedef enum _foo { _EOF = -1, OK, BAD_DATA, BAD_ALLOC, NOT_FOUND, BAD_KEY, BAD_FILE, EMPTY } Foo;

typedef enum _status { NIL, DELETED, BUSY } Status;

extern const char* errors[];
extern const char* statuses[];

#endif