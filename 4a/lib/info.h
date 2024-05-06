#ifndef LAB4A_4A_LIB_INFO_H_
#define LAB4A_4A_LIB_INFO_H_

typedef enum _compare { EQUAL, LESS, MORE } Compare;

typedef Compare (*fptr_compare)(void* left, void* right);

typedef struct _info Info;

#endif