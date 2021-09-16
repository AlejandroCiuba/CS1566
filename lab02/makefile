# Makefile needed to correctly compile test.c
# Created by Alejandro Ciuba

#Compiler and Flags, math.h must be linked
CC = gcc
CCFLAGS = -Wall
LDFLAGS = -lm
OBJFILES = test.o matrix_library/matrix_utility.o matrix_library/matrix_ops
TARGET= test
MAKE = make

all: subdirectory $(TARGET)

subdirectory:
	$(MAKE) -C matrix_library

$(TARGET): $(OBJFILES)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~