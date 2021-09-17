CC       = gcc 
CFLAGS   = -O3 -Wall 
LIBS      = -lXi -lXmu -lglut -lGLEW -lGLU -lm -lGL
OBJDIR   = ../matrix\ library
OBJS     = $(OBJDIR)/initShader.o $(OBJDIR)/matrix_ops.o $(OBJDIR)/matrix_utility.o

all: $(OBJDIR) template

template: template.c $(OBJS)
	$(CC) -o template template.c $(OBJS) $(CFLAGS) $(LIBS)

$(OBJDIR)/%.o: %.c %.h
	$(CC) -c @< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJDIR)/$(OBJS) ../template/template *~