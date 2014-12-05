CC:=clang
SRC_DIR:= src
INC_DIR:= inc
OBJ_DIR:= obj
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 -I $(INC_DIR)
LDFLAGS:=
SOURCES:= $(wildcard $(SRC_DIR)/*.c)
OBJECTS:= $(addprefix $(OBJ_DIR)/,$(notdir $(SOURCES:.c=.o)))
EXECUTABLE:= vm

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
