CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o $(OBJDIR)/shapes.o $(OBJDIR)/affine.o

all: matrix project01

.PHONY: project01
project01: project01.c $(OBJS)
	$(CC) -o project01 project01.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f project01 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 