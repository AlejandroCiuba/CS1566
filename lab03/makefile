CC = gcc 
CFLAGS = -O3 -Wall 
LIBS = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS     = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/shapes_library/affine.o

all: matrix lab03

.PHONY: lab03
lab03: lab03.c $(OBJS)
	$(CC) -o lab03 lab03.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f lab03 *~

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 