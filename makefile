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

DATENAME = gbhdate
DATEDIR = deps/$(DATENAME)

CFLAGS = -std=c++11 -Wall -Wextra -I$(INC) -I$(DATEDIR)/include
LFLAGS = -L$(DATEDIR)/lib -l$(DATENAME)

all: $(EXE)

$(EXE): $(OBJECTS)
	$(LINKER) -o $@ $(OBJECTS) $(LFLAGS)

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<



.PHONY: clean
clean:
	rm -f $(OBJECTS) $(EXE)
