#ifndef __MAIN__
#define __MAIN__

void dealloc(void* data);
void print(void* data);
int cmp(void* left, void* right);
int get_number(const char* format, void* number);
int read_key(size_t* key);
int read_data(size_t* data);

#endif