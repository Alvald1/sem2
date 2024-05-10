#ifndef LAB4A_4A_LIB_INFO_H_
#define LAB4A_4A_LIB_INFO_H_

#include "code_status.h"

typedef enum _compare { EQUAL, LESS, MORE } Compare;

typedef Compare (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void* data);

typedef struct _info Info;

Foo info_init(Info** info, fptr_compare compare, fptr_default print);

#endif