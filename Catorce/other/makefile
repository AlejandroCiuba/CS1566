#other makefile
#Created by Alejandro Ciuba
CC = gcc
CCFLAGS = -O3 -Wall -Werror -g

all: array_list file_reader

array_list: array_list.c
	$(CC) $(CCFLAGS) -c array_list.c

file_reader: file_reader.c
	$(CC) $(CCFLAGS) -c file_reader.c
	
clean:
	rm file_reader.o
	rm array_list.o