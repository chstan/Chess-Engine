CC = gcc

CFLAGS = -g -Wall -pedantic -O3 -std=c99

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = engine

#=============TARGETS=================
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core