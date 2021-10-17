#shapes_library makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall
OBJDIR = Catorce/matrix_library
OBJS = $(OBJDIR)/matrix_ops.o

all: shapes affine

shapes: shapes.c $(OBJS)
	$(CC) -o shapes shapes.c $(OBJS) $(CCFLAGS)

affine: affine.c
	$(CC) -o affine affine.c $(OBJS) $(CCFLAGS)

clean:
	rm shapes.o
	rm affine.o