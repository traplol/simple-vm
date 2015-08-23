CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
DBG_DIR:= dbg
BIN_DIR:= bin
LIB_DIR:= lib
MAIN:= main.o
TEST_DIR:= tests
INCLUDES:= -Iinc
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
DBG_OBJECTS:= $(addprefix $(OBJ_DIR)/$(DBG_DIR)/,$(notdir $(SOURCES:.c=.o)))
LIB_NAME:= libsimplevm.a
BINDINGS:= libsimplevm-vmbindings.so
EXECUTABLE:= vm

LIB_BUILD:= $(LIB_DIR)/$(LIB_NAME)
LIB_BINDINGS:= $(LIB_DIR)/$(BINDINGS)
EXE_BUILD:= $(BIN_DIR)/$(EXECUTABLE)


.PHONY: exe debug lib bindings clean

exe: EXE_BUILD:= $(BIN_DIR)/$(EXECUTABLE)
exe: $(EXE_BUILD)

debug: CFLAGS+=-DDEBUG_DISASSEM
debug: EXECUTABLE:=vm-debug
debug: build-exe

archive: clean $(LIB_BUILD)

bindings: clean $(LIB_BINDINGS)

$(EXE_BUILD): $(OBJECTS) | $(BIN_DIR)
	@echo "Building vm:'$@'..."
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$(EXECUTABLE) $^
	@echo "Done"

$(LIB_BUILD): $(filter-out $(OBJ_DIR)/$(MAIN),$(OBJECTS)) | $(LIB_DIR)
	@echo "Building lib:'$@'"
	ar -rcv $@ $^
	@echo "Done"

$(LIB_BINDINGS): CFLAGS+=-fPIC
$(LIB_BINDINGS): $(filter-out $(OBJ_DIR)/$(MAIN),$(OBJECTS)) | $(LIB_DIR)
	@echo "Building vm bindings."
	$(CC) -shared -Wl,-soname,$(BINDINGS) -o $@ $^
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
	rm -f $(LIB_BINDINGS)
	@echo "Done"
