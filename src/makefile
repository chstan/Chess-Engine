CC = clang

CFLAGS = -g -Wall -Wextra -Werror -fstack-protector -Wno-typedef-redefinition -Wno-empty-translation-unit -pedantic -O3

vpath %.h board
vpath %.h move
vpath %.h data-structures
vpath %.h aux
vpath %.h test
vpath %.h eval
vpath %.h search

vpath %.c board
vpath %.c move
vpath %.c data-structures
vpath %.c aux
vpath %.c test
vpath %.c eval
vpath %.c search

DIRS = board data-structures move test
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o) board.o linkedlist.o move.o movegen.o movegenhelpers.o notation.o random.o movegentest.o eval.o evalhelpers.o search.o zobrist.o uci.o
TARGET = engine
#DEPENDENCIES = $(SOURCES:.c=.d) board.d linkedlist.d move.d movegen.d movegenhelpers.d notation.d random.d movegentest.d eval.d evalhelpers.d search.d

#=============TARGETS=================
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

#%.d: %.c
#	$(CC) -MM $< >> $@

clean:
	@rm -f $(TARGET) $(OBJECTS) $(DEPENDENCIES) core


include $(DEPENDENCIES)
