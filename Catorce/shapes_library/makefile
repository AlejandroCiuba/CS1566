#shapes_library makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall
OBJDIR = ../matrix_library
OBJS = $(OBJDIR)/matrix_ops.o

all: shapes affine

shapes: shapes.c $(OBJS)
	$(CC) $(CCFLAGS) -c shapes.c $(OBJS) 

affine: affine.c
	$(CC) $(CCFLAGS) -c affine.c $(OBJS) 

clean:
	rm shapes.o
	rm affine.o