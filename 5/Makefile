CFLAGS = -Wall -Wextra -Werror
CC = clang
SAN = -g -fsanitize=undefined
LIB_LIB = hash_table
LIB_DIR = lib
OBJECT_DIR = object
EXECUTABLE = bin
LDFLAGS = -l$(LIB_LIB) -L.
OPTIM = -O0
LIB_NAME = 
LIB_SOURCES = $(wildcard $(LIB_LIB)/*.c)
LIB_OBJECTS = $(LIB_SOURCES:%.c=$(OBJECT_DIR)/%.o)
SOURCES = $(wildcard *.c $(LIB_DIR)/*.c)
OBJECTS = $(SOURCES:%.c=$(OBJECT_DIR)/%.o)
DEP_FLAGS = -MMD -MP
COV_FLAG =

.PHONY: all lib debug asan msan afl static clean cov valgrind fuzz compile

all:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_clean" SAN=""

lib:
	$(MAKE) build LIB_NAME="lib$(LIB_LIB)" CFLAGS="-g" SAN=""

debug:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_debug" CFLAGS="-g" SAN=""

asan:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_asan" SAN="$(SAN),address"

gprof:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_gprof" SAN="" CC="gcc" CFLAGS="-pg"

msan:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_msan" SAN="$(SAN),memory"

afl:
	$(MAKE) compile EXECUTABLE="$(EXECUTABLE)_afl" CC="$(AFL_PATH)afl-clang-fast" SAN="$(SAN),address"

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
	echo '-' > in/-
	echo '1' > in/1
	echo '-3' > in/-3
	echo '2' > in/2
	echo '4' > in/4
	echo '9' > in/9
	echo '100' > in/100
	echo 'a' > in/a
	echo 'A' > in/A
	echo 'd' > in/d
	echo 'D' > in/D
	echo 'c' > in/c
	echo 'C' > in/C
	echo 'b' > in/b
	echo 'B' > in/B
	echo 'f' > in/f
	echo 'g' > in/g
	echo 'i' > in/i
	echo 'gr' > in/gr
	echo 'sf' > in/sf
	echo 'grp' > in/grp
	echo 'd2p' > in/d2p
	echo 'i123' > in/i123
	echo 'r456' > in/r456
	echo '' > in/empty
	echo 'xyz' > in/invalid
	echo '123' > in/nocommand
	tmux new-session -d -s my_session -n Window1 '$(AFL_PATH)afl-fuzz -i in -o out -M master -x utf8.dict -P -- ./$(EXECUTABLE)_afl'
	tmux new-window -t my_session:1 -n Window2 '$(AFL_PATH)afl-fuzz -i in -o out -S slave1 -x utf8.dict -P -- ./$(EXECUTABLE)_afl'
	tmux new-window -t my_session:2 -n Window3 '$(AFL_PATH)afl-fuzz -i in -o out -S slave2 -x utf8.dict -P -- ./$(EXECUTABLE)_afl'
	tmux new-window -t my_session:3 -n Window4 '$(AFL_PATH)afl-fuzz -i in -o out -S slave3 -x utf8.dict -P -- ./$(EXECUTABLE)_afl'
	tmux select-window -t my_session:0
	tmux attach-session -t my_session

compile: $(EXECUTABLE)

build: $(LIB_NAME).a

$(LIB_NAME).a: $(LIB_OBJECTS)
	ar rcs $@ $(LIB_OBJECTS)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@ $(CFLAGS) $(SAN) $(LDFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(OPTIM) $(SAN) $(COV_FLAG) $(DEP_FLAGS) -c $< -o $@

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

clean:
	rm -rf $(EXECUTABLE)* $(OBJECT_DIR) *.plist in gmon.out output* result*
