#ifndef LAB3B_3B_LIB_INFO_H_
#define LAB3B_3B_LIB_INFO_H_

#include <stddef.h>

#include "code_status.h"

typedef int (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void*);

typedef struct _info Info;

Foo info_init(Info** info, fptr_compare compare, fptr_default key_dealloc, fptr_default data_dealloc,
              fptr_default key_print, fptr_default data_print, size_t key_size);
void info_dealloc(Info* info);

#endif