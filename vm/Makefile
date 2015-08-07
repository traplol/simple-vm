CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
DBG_DIR:= dbg
BIN_DIR:= bin
LIB_DIR:= lib
MAINS:= main.o
TEST_DIR:= tests
INCLUDES:= -Iinc
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
DBG_OBJECTS:= $(addprefix $(OBJ_DIR)/$(DBG_DIR)/,$(notdir $(SOURCES:.c=.o)))
LIB_NAME:= libsimplevm.a
EXECUTABLE:= vm

LIB_BUILD:= $(LIB_DIR)/$(LIB_NAME)
EXE_BUILD:= $(BIN_DIR)/$(EXECUTABLE)


.PHONY: all clean build debug-asm lib

all: build-lib build-exe

debug-asm: CFLAGS+=-DDEBUG_DISASSEM
debug-asm: EXECUTABLE:=vm-debug
debug-asm: build-exe

build-exe: EXE_BUILD:= $(BIN_DIR)/$(EXECUTABLE)
build-exe: $(EXE_BUILD)

build-lib: $(LIB_BUILD)

$(LIB_BUILD): $(filter-out $(OBJ_DIR)/$(MAINS),$(OBJECTS)) | $(LIB_DIR)
	@echo "Building lib:'$@'"
	ar -rcv $@ $^
	@echo "Done"

$(EXE_BUILD): $(OBJECTS) | $(BIN_DIR)
	@echo "Building vm:'$@'..."
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(EXECUTABLE) $^
	@echo "Done"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "Building obj:'$@'"
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR): 
	mkdir -p $(LIB_DIR)

clean:
	@echo "Cleaning up VM."
	rm -f $(OBJECTS)
	rm -f $(BIN_DIR)/$(EXECUTABLE)*
	rm -f $(LIB_DIR)/$(LIB_NAME)
	@echo "Done"
