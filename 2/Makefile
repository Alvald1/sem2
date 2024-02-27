CFLAGS = -Wall -Wextra -Werror

ASAN = -fsanitize=address,undefined -g

LIB_DIR = lib
EXECUTABLE = bin

TARGET = vector

OPTIM = -O0

LDFLAGS = 

all: compile

patient.o: $(LIB_DIR)/patient.c $(LIB_DIR)/patient.h
	clang -c $< $(ASAN) $(CFLAGS) $(OPTIM)

vector.o: $(LIB_DIR)/vector.c $(LIB_DIR)/dequeue.h
	clang -c $< $(ASAN) $(CFLAGS) $(OPTIM)

list.o: $(LIB_DIR)/list.c $(LIB_DIR)/dequeue.h
	clang -c $< $(ASAN) $(CFLAGS) $(OPTIM)

main.o: main.c
	clang -c $< $(ASAN) $(CFLAGS) $(OPTIM)

compile: patient.o main.o $(TARGET).o
	clang $? -o $(EXECUTABLE) $(ASAN) $(LDFLAGS) $(OPTIM) 

static: 
	clang --analyze -Xanalyzer -analyzer-output=html *.c $(LIB_DIR)/*.c

formating:
	clang-format *.c -i -style=file

clean:
	rm -rf $(EXECUTABLE)* *plist *.o

help:
	@echo "Available targets:"
	@echo "  all      : Build the executable with default settings"
	@echo "  compile  : Build the executable"
	@echo "  debug    : Build the executable with debugging information"
	@echo "  asan     : Build the executable with AddressSanitizer"
	@echo "  static   : Run static analysis and generate HTML reports"
	@echo "  clean    : Remove generated files"
	@echo "  formating: Format source code using clang-format"
	@echo "  help     : Display this help message"
	
