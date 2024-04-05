#ifndef __INFO__
#define __INFO__

typedef int (*fptr_compare)(void* left, void* right);
typedef void (*fptr_default)(void* data);

typedef struct _info {
    fptr_compare compare;
    fptr_default key_print, data_print, key_dealloc, data_dealloc;
} Info;

int __info_valid(Info* info);

#endif