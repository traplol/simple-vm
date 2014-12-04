CC:=clang
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99
LDFLAGS:=
SOURCES:= vm.c helpers.c irm.c irr.c smart-compile.c
OBJECTS:= $(SOURCES:.c=.o)
EXECUTABLE:= vm

.PHONY: all clean compile test

all: compile $(EXECUTABLE) test

compile:
	$(CC) $(CFLAGS) $(CLIBS) -c $(SOURCES)

$(EXECUTABLE): compile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(EXECUTABLE)

test: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJECTS)
