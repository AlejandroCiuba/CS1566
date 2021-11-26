CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/matrix_library/matrix_utility.o\
$(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/shapes_library/affine.o\
$(OBJDIR)/other/file_reader.o $(OBJDIR)/other/array_list.o\
$(OBJDIR)/camera/camera.o

all: matrix project04

.PHONY: project04
project04: project04.c $(OBJS)
	$(CC) -o project04 project04.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f project04 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 