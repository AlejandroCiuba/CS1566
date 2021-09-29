#Matrix Library Makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall

all: initShader matrix_utility matrix_ops matrix_shapes

initShader: initShader.c
	$(CC) $(CCFLAGS) -c initShader.c

matrix_utility: matrix_utility.c
	$(CC) $(CCFLAGS) -c matrix_utility.c

matrix_ops: matrix_ops.c
	$(CC) $(CCFLAGS) -c matrix_ops.c

matrix_shapes: matrix_shapes.c
	$(CC) $(CCFLAGS) -c matrix_shapes.c

clean:
	rm matrix_ops.o 
	rm matrix_utility.o 
	rm matrix_shapes.o
	rm initShader.o
