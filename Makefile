// Makefile


/*
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
INCLUDES = -Iinclude
SRCDIR = src
OBJDIR = obj
TARGET = rpg


SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))


all: dirs $(TARGET)


dirs:
mkdir -p $(OBJDIR)


$(OBJDIR)/%.o: $(SRCDIR)/%.c
$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


$(TARGET): $(OBJECTS)
$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)


clean:
rm -rf $(OBJDIR) $(TARGET)


.PHONY: all clean dirs
*/