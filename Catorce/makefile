#Catorce Library Makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall

all: matrix_library shapes_library other

matrix_library:
	$(MAKE) -C matrix_library

shapes_library:
	$(MAKE) -C shapes_library

other:
	$(MAKE) -C other

clean: clean_ml clean_sl clean_o

clean_ml:
	$(MAKE) clean -C matrix_library

clean_sl:
	$(MAKE) clean -C shapes_library

clean_o:
	$(MAKE) clean -C other