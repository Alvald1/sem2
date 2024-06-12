#ifndef LAB3B_3B_LIB_INFO_H_
#define LAB3B_3B_LIB_INFO_H_

#include <stddef.h>

#include "code_status.h"

typedef enum _compare { EQUAL, LESS, MORE } Compare;

typedef Compare (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void*);

typedef struct _hash_info Hash_Info;

Hash_Foo info_init(Hash_Info** info, fptr_compare compare, fptr_default key_dealloc, fptr_default data_dealloc,
                   fptr_default key_print, fptr_default data_print, size_t data_size);
void info_dealloc(Hash_Info* info);

#endif