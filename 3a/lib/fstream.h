#ifndef LAB3A_3A_LIB_FSTREAM_H_
#define LAB3A_3A_LIB_FSTREAM_H_

#include "code_status.h"
#include "info.h"

typedef struct _table Table;

Foo read_from_file(char* f_name, Table** table, Info* info);

#endif