# The compiler to use
CC=gcc

# Options flags to pass to the compiler
CFLAGS=-c -Wall -pedantic

# Src and Build (i.e. obj) directories
VPATH=src
BUILDDIR=obj

# Compile each .c src file to a .o file without linking 
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# Link all .o files with gcc to get the final executable
$(BUILDDIR)/lab1: $(BUILDDIR)/main.o $(BUILDDIR)/transmit.o $(BUILDDIR)/layer1.o $(BUILDDIR)/layer2.o $(BUILDDIR)/layer3.o $(BUILDDIR)/layer4.o $(BUILDDIR)/receive.o
	$(CC) $(BUILDDIR)/main.o $(BUILDDIR)/transmit.o $(BUILDDIR)/layer1.o $(BUILDDIR)/layer2.o $(BUILDDIR)/layer3.o $(BUILDDIR)/layer4.o $(BUILDDIR)/receive.o -o $(BUILDDIR)/lab1


clean:
	rm -rf $(BUILDDIR)/*o $(BUILDDIR)/lab1

