# makefile for this project, copy from darknet.
TEST=0

# source file directory (*.c) for make to search
VPATH=./src/
# object file directory (*.o)
OBJDIR=./obj/
# header file directory
HEADER=-Iinclude/ -I3rd
# external library directory
LIB=
# link option
LINK=-lm
# macros
MACRO=
# executable file
EXEC=main

# compiler
CC=g++
# compiler option
CFLAGS=-O3 -Wall -Wfatal-errors -Wno-unused-result -Wno-unknown-pragmas -fopenmp

# OBJ=main.o image.o
OBJ=main.o image.o
ifeq ($(TEST), 1)
	OBJ += test.o
endif

OBJS = $(addprefix $(OBJDIR), $(OBJ))
DEPS = $(wildcard include/*.h*) makefile

all: $(OBJDIR) $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(HEADER) $(LIB) $(MACRO) $(CFLAGS) $^ -o $@ $(LINK)

$(OBJDIR)%.o: %.cc $(DEPS)
	$(CC) $(HEADER) $(LIB) $(MACRO) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY: clean run

clean:
	rm -rf $(OBJS) $(EXEC)

run: clean all
	./$(EXEC)