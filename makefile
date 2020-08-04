INC = include
SRC = src
OBJ = obj
BIN = bin
EXE = $(BIN)/finances.out

CC = g++
LINKER = g++

OBJECTS = $(OBJ)/finances.o              \
		  $(OBJ)/ioandconstructors.o     \
		  $(OBJ)/select.o     \
		  $(OBJ)/main.o                  \
		  $(OBJ)/utilities.o

DATENAME = gbhdate
DATEDIR = deps/$(DATENAME)

CFLAGS = -std=c++11 -Wall -Wextra -I$(INC)
LFLAGS =

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS) $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<



.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXE)
