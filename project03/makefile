CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS     = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/matrix_library/matrix_utility.o $(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/shapes_library/affine.o $(OBJDIR)/other/file_reader.o

all: matrix project03

.PHONY: project03
project03: project03.c $(OBJS)
	$(CC) -o project03 project03.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f project03 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 