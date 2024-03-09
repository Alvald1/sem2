CFLAGS = -Wall -Wextra -Werror

SAN = -g -fsanitize=undefined

LIB_DIR = lib
OBJECT_DIR = object
EXECUTABLE = bin

TARGET = list

OPTIM = -O0

LDFLAGS = 

CC = clang

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

.PHONY: all compile preparation linker debug asan msan static clean formatting help 

$(OBJECT_DIR)/task.o: $(LIB_DIR)/task.c $(LIB_DIR)/task.h $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM)

$(OBJECT_DIR)/patient.o: $(LIB_DIR)/patient.c $(LIB_DIR)/patient.h $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM)

$(OBJECT_DIR)/vector.o: $(LIB_DIR)/vector.c $(LIB_DIR)/dequeue.h $(LIB_DIR)/vector.h $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM)

$(OBJECT_DIR)/list.o: $(LIB_DIR)/list.c $(LIB_DIR)/dequeue.h $(LIB_DIR)/list.h $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM)

$(OBJECT_DIR)/main.o: main.c $(LIB_DIR)/code_status.h
	$(CC) -c $< -o $@ $(SAN) $(CFLAGS) $(OPTIM)

compile: preparation linker

preparation:
	mkdir -p $(LIB_DIR)
	mkdir -p $(OBJECT_DIR)

linker: $(OBJECT_DIR)/patient.o $(OBJECT_DIR)/main.o $(OBJECT_DIR)/task.o $(OBJECT_DIR)/$(TARGET).o	
	$(CC) $? -o $(EXECUTABLE) $(SAN) $(LDFLAGS) $(OPTIM) 

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

formating:
	clang-format *.c -i -style=file

clean:
	rm -rf $(EXECUTABLE)* *plist $(OBJECT_DIR)/*.o

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
	
