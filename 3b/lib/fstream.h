#ifndef LAB3B_3B_LIB_FSTREAM_H_
#define LAB3B_3B_LIB_FSTREAM_H_

#include "code_status.h"
#include "info.h"
#include "table.h"

Foo table_import(Table** table, Info* info, const char* file_name);
Foo table_export(Table* table, const char* file_name);

#endif