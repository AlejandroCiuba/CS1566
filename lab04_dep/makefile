CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS     = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/shapes_library/affine.o

all: matrix lab04

.PHONY: lab04
lab04: lab04.c $(OBJS)
	$(CC) -o lab04 lab04.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f lab04 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 