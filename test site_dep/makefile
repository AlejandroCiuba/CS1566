CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o $(OBJDIR)/shapes.o

all: matrix test

.PHONY: test
test: test_site.c $(OBJS)
	$(CC) -o test test_site.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f test *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 