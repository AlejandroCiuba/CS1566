# Makefile for lab 5
# Created by: Alejandro Ciuba

CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS     = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS     = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/shapes_library/affine.o $(OBJDIR)/other/file_reader.o

all: matrix lab05

lab05: lab05.c $(OBJS)
	$(CC) -o lab05 lab05.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f lab05 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 