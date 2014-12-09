CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
BIN_DIR:= bin
INCLUDES:= -Iinc -I../vm/inc
CFLAGS:= -O0 -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
EXECUTABLE:= $(BIN_DIR)/assembler

TESTASM:= test.asm

.PHONY: all clean asm disasm exec

all: $(EXECUTABLE) asm

asm: $(EXECUTABLE)
	./bin/assembler $(TESTASM) -o $(TESTASM:.asm=.bin)

disasm:
	../vm/bin/vm -d $(TESTASM:.asm=.bin)

exec: asm
	../vm/bin/vm $(TESTASM:.asm=.bin)


$(INC_DIRS): 
	CFLAGS += -I $@

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ -L ../vm/lib -lsimplevm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
