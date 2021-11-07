#Catorce Library Makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall

all: matrix_library shapes_library camera other

.PHONY: matrix_library
.PHONY: shapes_library
.PHONY: camera
.PHONY: other

matrix_library:
	$(MAKE) -C matrix_library

shapes_library:
	$(MAKE) -C shapes_library

camera:
	$(MAKE) -C camera

other:
	$(MAKE) -C other

clean: clean_ml clean_sl clean_cam clean_o

clean_ml:
	$(MAKE) clean -C matrix_library

clean_sl:
	$(MAKE) clean -C shapes_library

clean_cam:
	$(MAKE) clean -C camera

clean_o:
	$(MAKE) clean -C other