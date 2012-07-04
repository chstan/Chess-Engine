CC = clang

CFLAGS = -g -Wall -pedantic -O0

vpath %.h board
vpath %.h move
vpath %.h data-structures
vpath %.h aux

vpath %.c board
vpath %.c move
vpath %.c data-structures
vpath %.c aux

DIRS = board data-structures move
SOURCES = $(wildcard *.c) 
OBJECTS = $(SOURCES:.c=.o) board.o linkedlist.o move.o movegen.o movegenhelpers.o notation.o random.o
TARGET = engine

#=============TARGETS=================
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core