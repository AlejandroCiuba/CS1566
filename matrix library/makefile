#Flags used by the rest of the makefile use $(FLAGNAME) to use flag
CC = gcc
CFLAGS = -Wall
#Linking stuff like -lm
LDFLAGS = -lm
#Will recompile if .c != .o or if .o does not exist
OBJFILES = test_lib.o matrix_utility.o matrix_ops.o
#Name of the main file
TARGET = test

#What to do when we type ONLY "make"
all: $(TARGET)

#Tab is necessary
$(TARGET): $(OBJFILES)
	$(CC) $(CCFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

#Type make clean to remove object files, main file, and all ~ files
clean:
	rm -f $(OBJFILES) $(TARGET) *~