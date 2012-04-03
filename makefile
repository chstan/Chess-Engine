CC = gcc

#Although this will be changed, do not optimize
CFLAGS = -g -Wall -O0 -std=c99

SOURCES = engine.c alert.c cmd.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = engine

#=============TARGETS=================
$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core