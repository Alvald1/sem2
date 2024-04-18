CFLAGS = -Wall -Wextra -Werror 

CC_AFL = $(AFL_PATH)afl-clang

SAN = -g -fsanitize=undefined

LIB_DIR = lib
OBJECT_DIR = object
EXECUTABLE = bin

TARGET = without_iter

OPTIM = -O0

LDFLAGS = -lreadline

CC = clang

COV_FLAG = 

.PHONY: compile preparation linker debug asan msan static clean formatting help 

all: SAN = 
all: EXECUTABLE := $(EXECUTABLE)_clean 
all: compile

debug: SAN = 
debug: EXECUTABLE := $(EXECUTABLE)_debug 
debug: CFLAGS = -g
debug: compile

asan: SAN := $(SAN),address
asan: EXECUTABLE := $(EXECUTABLE)_asan
asan: compile

msan: SAN := $(SAN),memory
msan: EXECUTABLE := $(EXECUTABLE)_msan
msan: compile

afl: CC = $(CC_AFL)
afl: asan

coverage: SAN =
coverage: COV_FLAG = -fprofile-instr-generate -fcoverage-mapping
coverage: EXECUTABLE := $(EXECUTABLE)_cov
coverage: compile
	./get_cover.sh
	llvm-profdata merge -sparse default.profraw -o foo.profdata
	llvm-cov show  $(EXECUTABLE) -instr-profile=foo.profdata
	llvm-cov report $(EXECUTABLE) -instr-profile=foo.profdata
	rm foo.profdata default.profraw 

compile: preparation linker

preparation:
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJECT_DIR)

linker: $(OBJECT_DIR)/main.o $(OBJECT_DIR)/info.o $(OBJECT_DIR)/item.o $(OBJECT_DIR)/table.o $(OBJECT_DIR)/fstream.o $(OBJECT_DIR)/code_status.o
	$(CC) $? -o $(EXECUTABLE) $(SAN) $(LDFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/main.o: main.c main.h $(LIB_DIR)/code_status.h 
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/code_status.o: $(LIB_DIR)/code_status.c $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/info.o: $(LIB_DIR)/info.c $(LIB_DIR)/code_status.h $(LIB_DIR)/info.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/fstream.o: $(LIB_DIR)/fstream.c $(LIB_DIR)/code_status.h $(LIB_DIR)/fstream.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/item.o: $(LIB_DIR)/item.c $(LIB_DIR)/code_status.h $(LIB_DIR)/item.h $(LIB_DIR)/item_lib.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

$(OBJECT_DIR)/table.o: $(LIB_DIR)/table.c $(LIB_DIR)/code_status.h $(LIB_DIR)/table.h $(LIB_DIR)/table_lib.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM) $(COV_FLAG)

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

formating:
	clang-format *.c -i -style=file

clean:
	rm -rf $(EXECUTABLE)* *plist $(OBJECT_DIR)/*.o $(OBJECT_DIR)/*.gcda $(OBJECT_DIR)/*.gcno *.profraw

help:
	@echo "Available targets:"
	@echo "  all      : Build the executable with default settings"
	@echo "  linker  : Build the executable"
	@echo "  debug    : Build the executable with debugging information"
	@echo "  asan     : Build the executable with AddressSanitizer"
	@echo "  msan     : Build the executable with MemorySanitizer"
	@echo "  static   : Run static analysis and generate HTML reports"
	@echo "  clean    : Remove generated files"
	@echo "  formating: Format source code using clang-format"
	@echo "  help     : Display this help message"
	
