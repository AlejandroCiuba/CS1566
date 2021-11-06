# Test site makefile
# Created by Alejandro Ciuba

CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../Catorce
OBJS_CAT     = $(OBJDIR)/matrix_library/initShader.o $(OBJDIR)/matrix_library/matrix_ops.o $(OBJDIR)/matrix_library/matrix_utility.o $(OBJDIR)/shapes_library/shapes.o $(OBJDIR)/shapes_library/affine.o $(OBJDIR)/other/file_reader.o $(OBJDIR)/camera/camera.o
OBJS = ../other_programs/array_list.o

all: matrix test 

matrix:
	$(MAKE) -C $(OBJDIR)

test: test.c $(OBJS)
	$(CC) $(CFLAGS) -o test test.c $(OBJS) $(LIBS)

clean:
	rm -f *.o

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 