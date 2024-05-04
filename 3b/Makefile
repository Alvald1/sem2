CFLAGS = -Wall -Wextra -Werror
CC = clang
SAN = -g -fsanitize=undefined
LIB_DIR = lib
OBJECT_DIR = object
EXECUTABLE = bin
LDFLAGS = -lreadline
OPTIM = -Og
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

cov: 
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_cov" SAN="" COV_FLAG="-fprofile-instr-generate -fcoverage-mapping"
	./get_cover.sh
	llvm-profdata merge -sparse default.profraw -o foo.profdata
	llvm-cov show  $(EXECUTABLE)_cov -instr-profile=foo.profdata
	llvm-cov report $(EXECUTABLE)_cov -instr-profile=foo.profdata
	llvm-cov show $(EXECUTABLE)_cov -instr-profile=foo.profdata -format=html -output-dir=report_coverage
	rm foo.profdata default.profraw 

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./bin_debug

fuzz: afl
	rm -rf in out
	mkdir in out
	echo 1 >> in/1
	echo "l" >> in/2
	echo "ф" >> in/3
	tmux new-session -d -s my_session -n Window1 '$(AFL_PATH)afl-fuzz -i in -o out -M master -x utf8.dict -- ./$(BIN)_afl'
	tmux new-window -t my_session:1 -n Window2 '$(AFL_PATH)afl-fuzz -i in -o out -S slave1 -x utf8.dict -- ./$(BIN)_afl'
	tmux new-window -t my_session:2 -n Window3 '$(AFL_PATH)afl-fuzz -i in -o out -S slave2 -x utf8.dict -- ./$(BIN)_afl'
	tmux new-window -t my_session:3 -n Window4 '$(AFL_PATH)afl-fuzz -i in -o out -S slave3 -x utf8.dict -- ./$(BIN)_afl'
	tmux select-window -t my_session:0
	tmux attach-session -t my_session

compile: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(SAN) $(LDFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OPTIM) $(SAN) $(COV_FLAG) $(DEP_FLAGS) -c $< -o $@

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

clean:
	rm -rf $(EXECUTABLE)* $(OBJECT_DIR) *.plist in
