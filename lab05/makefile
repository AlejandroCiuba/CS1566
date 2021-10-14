# Makefile for lab 5
# Created by: Alejandro Ciuba

CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS     = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o 

all: matrix lab05

lab05: lab05
	$(CC) -o lab05 lab05.c $(OBJS) $(CFLAGS) $(LIBS)

matrix:
	$(MAKE) -C $(OBJDIR)

clean:
	rm -f lab05 *~ 

clean_all: clean
	$(MAKE) clean -C $(OBJDIR) 