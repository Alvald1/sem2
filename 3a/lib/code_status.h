#ifndef __CODE_STATUS__
#define __CODE_STATUS__

typedef enum _foo { OK, BAD_ALLOC, OVERFLOW, BAD_DATA, BAD_KEY, BAD_COMP, NOT_FOUND, BAD_FILE, BAD_NAME } Foo;

extern const char* errors_my[];

#endif