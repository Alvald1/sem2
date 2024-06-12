#include "word_search.h"

#include <stdio.h>
#include <string.h>

#include "info_lib.h"
#include "numbers.h"
#include "rb_lib.h"
#include "readline.h"

Foo
word_search(RB* rb, const char* file_name) {
    __rb_dealloc(rb);
    rb->root = rb->nil;
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        return BAD_FILE;
    }
    size_t cnt_lines = 0;
    char* line = NULL;
    char* word = NULL;
    void* tmp = NULL;
    while ((line = readline(file, "")) != NULL) {
        cnt_lines++;
        word = strtok(line, " ");
        do {
            if (rb_insert(rb, (tmp = strdup(word)), gen_number(cnt_lines)) == DUPLICATE) {
                free(tmp);
            }
        } while ((word = strtok(NULL, " ")) != NULL);
        free(line);
    }
    return OK;
}