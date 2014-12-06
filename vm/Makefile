CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
BIN_DIR:= bin
LIB_DIR:= lib
MAINS:= main.o
TEST_DIR:= test
INCLUDES:= -Iinc
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
LIB_NAME:= libsimplevm.a
EXECUTABLE:= vm

.PHONY: all clean tests

all: $(LIB_NAME)

tests:
	cd $(TEST_DIR); make

$(LIB_NAME): $(filter-out $(OBJ_DIR)/$(MAINS),$(OBJECTS)) | $(LIB_DIR)
	ar -rcv $(LIB_DIR)/$@ $^

$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $(BIN_DIR)/$@ $^

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
	rm -f $(LIB_NAME)
