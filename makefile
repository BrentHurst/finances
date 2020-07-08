INC = include
SRC = src
OBJ = obj
BIN = bin
EXE = $(BIN)/finances.out

CC = g++
LINKER = g++

OBJECTS = $(OBJ)/finances.o    \
		  $(OBJ)/main.o        \
		  $(OBJ)/utilities.o

CFLAGS = -std=c++11 -Wall -Wextra

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<



.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXE)
