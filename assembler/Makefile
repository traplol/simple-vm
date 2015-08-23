CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
BIN_DIR:= bin
LIB_DIR:= lib
MAIN:= assembler.o
INCLUDES:= -Iinc -I../vm/inc
CFLAGS:= -O0 -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
BINDINGS:= libsimplevm-assemblerbindings.so
LIB_BINDINGS:= $(LIB_DIR)/$(BINDINGS)
EXECUTABLE:= $(BIN_DIR)/assembler

.PHONY: exe bindings vm-archive clean

exe: vm-archive $(EXECUTABLE)

bindings: clean $(LIB_BINDINGS)

$(INC_DIRS): 
	CFLAGS += -I $@

$(LIB_BINDINGS): CFLAGS+=-fPIC
$(LIB_BINDINGS): $(filter-out $(OBJ_DIR)/$(MAIN),$(OBJECTS)) | $(LIB_DIR)
	@cd ../vm && make bindings
	@echo "Building assembler bindings."
	$(CC) -shared -Wl,-soname,$(BINDINGS) -L ../vm/lib -lsimplevm-vmbindings -o $@ $^
	@echo "Done"

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	@cd ../vm && make archive
	$(CC) $(LDFLAGS) -o $@ $^ -L ../vm/lib -lsimplevm

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(LIB_DIR): 
	mkdir -p $(LIB_DIR)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
	rm -f $(LIB_BINDINGS)
