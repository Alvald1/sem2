#ifndef LAB3B_3B_LIB_FSTREAM_H_
#define LAB3B_3B_LIB_FSTREAM_H_

#include "code_status.h"
#include "info.h"
#include "table.h"

Hash_Foo table_import(Table** table, Hash_Info* info, const char* file_name);
Hash_Foo table_export(Table* table, const char* file_name);

#endif