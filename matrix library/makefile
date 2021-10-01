#Matrix Library Makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall

all: initShader matrix_utility matrix_ops affine shapes file_reader

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

file_reader: file_reader.c
	$(CC) $(CCFLAGS) -c file_reader.c

clean:
	rm shapes.o
	rm matrix_ops.o 
	rm matrix_utility.o 
	rm affine.o
	rm initShader.o
	rm file_reader.o
