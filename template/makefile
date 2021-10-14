CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS     = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o

all: matrix template

.PHONY: template
template: template.c $(OBJS)
	$(CC) -o template template.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f template *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 