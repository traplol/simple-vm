CC:=clang
INCLUDES:= -I../inc
CFLAGS:= -Werror -Wall -pedantic -pedantic-errors -Wextra -g -std=c99 $(INCLUDES)
LDFLAGS:=
SOURCES:= binwriter.c
EXECUTABLE:= binwriter

.PHONY: all clean

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE) -L ../lib -lsimplevm

clean:
	rm -f $(EXECUTABLE)
