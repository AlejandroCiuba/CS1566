CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o $(OBJDIR)/shapes.o $(OBJDIR)/affine.o $(OBJDIR)/file_reader.o

all: matrix project02

.PHONY: project02
project02: project02.c $(OBJS)
	$(CC) -o project02 project02.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f project02 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 