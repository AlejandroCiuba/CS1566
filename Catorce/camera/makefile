#camera makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall -Werror -g

all: camera lighting

camera: camera.c
	$(CC) $(CCFLAGS) -c camera.c

lighting: lighting.c
	$(CC) $(CCFLAGS) -c lighting.c

clean:
	rm camera.o
	rm lighting.o