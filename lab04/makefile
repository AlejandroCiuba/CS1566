CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o $(OBJDIR)/matrix_shapes.o $(OBJDIR)/shapes.o

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