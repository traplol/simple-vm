CC:=clang
SRC_DIR:= src
OBJ_DIR:= obj
INCLUDES:= -Iinc -I../vm/inc
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
EXECUTABLE:= assembler

.PHONY: includes all clean

all: $(EXECUTABLE)

$(INC_DIRS): 
	CFLAGS += -I $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) 
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
