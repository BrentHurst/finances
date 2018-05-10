EXECUTABLE = finances.exe
SOURCE = $(wildcard *.cpp)
HEADER = $(wildcard *.h)

CC = g++
LANGUAGE = cpp
INCLUDES =
LIBS =
CFLAGS = -g -Wall $(INCLUDES:%=-I%)
LFLAGS = -g

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE) $(HEADER) makefile
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)
