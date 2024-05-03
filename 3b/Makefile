CFLAGS = -Wall -Wextra -Werror
CC = clang
SAN = -g -fsanitize=undefined
LIB_DIR = lib
OBJECT_DIR = object
EXECUTABLE = bin
LDFLAGS = -lreadline
OPTIM = -O0
SOURCES = $(wildcard *.c $(LIB_DIR)/*.c)
OBJECTS = $(SOURCES:%.c=$(OBJECT_DIR)/%.o)
DEP_FLAGS = -MMD -MP
COV_FLAG =

.PHONY: all debug asan msan afl static clean

all:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_clean" SAN=""

debug:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_debug" CFLAGS="-g" SAN=""

asan:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_asan" SAN="$(SAN),address"

msan:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_msan" SAN="$(SAN),memory"

afl:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_afl" CC="$(AFL_PATH)afl-clang" SAN="$(SAN),address"

coverage: 
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_cov" SAN="" COV_FLAG="-fprofile-instr-generate -fcoverage-mapping"
	./get_cover.sh
	llvm-profdata merge -sparse default.profraw -o foo.profdata
	llvm-cov show  $(EXECUTABLE) -instr-profile=foo.profdata
	llvm-cov report $(EXECUTABLE) -instr-profile=foo.profdata
	rm foo.profdata default.profraw 

compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(SAN) $(LDFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OPTIM) $(SAN) $(COV_FLAG) $(DEP_FLAGS) -c $< -o $@

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

clean:
	rm -rf $(EXECUTABLE)* $(OBJECT_DIR) *.plist
