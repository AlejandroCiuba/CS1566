# Makefile needed to correctly compile test.c
# Created by Alejandro Ciuba

##Compiler and Flags, math.h must be linked
CC = gcc
CCFLAGS = -Wall
LDFLAGS = -lm
OBJFILES = test.o matrix_utility.o matrix_ops
TARGET = test

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CCFLAGS) -o $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~