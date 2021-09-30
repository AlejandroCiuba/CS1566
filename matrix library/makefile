#Matrix Library Makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall

all: initShader matrix_utility matrix_ops affine shapes

initShader: initShader.c
	$(CC) $(CCFLAGS) -c initShader.c

matrix_utility: matrix_utility.c
	$(CC) $(CCFLAGS) -c matrix_utility.c

matrix_ops: matrix_ops.c
	$(CC) $(CCFLAGS) -c matrix_ops.c

affine: affine.c
	$(CC) $(CCFLAGS) -c affine.c

shapes: shapes.c
	$(CC) $(CCFLAGS) -c shapes.c

clean:
	rm shapes.o
	rm matrix_ops.o 
	rm matrix_utility.o 
	rm affine.o
	rm initShader.o
