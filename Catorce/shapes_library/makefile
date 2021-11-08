#shapes_library makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall -Werror -g

all: shapes affine

shapes: shapes.c $(OBJS)
	$(CC) $(CCFLAGS) -c shapes.c 

affine: affine.c
	$(CC) $(CCFLAGS) -c affine.c

clean:
	rm shapes.o
	rm affine.o