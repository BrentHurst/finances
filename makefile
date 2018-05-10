EXECUTABLE = finances.exe
SOURCE = $(wildcard *.cpp)
HEADER = $(wildcard *.h)
OBJECTS = $(wildcard *.o)

CC = g++
LANGUAGE = cpp
INCLUDES =
LIBS =
CFLAGS = -g -Wall $(INCLUDES:%=-I%)
LFLAGS = -g

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE) $(HEADER) *.o
	$(CC) $(LFLAGS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.$(LANGUAGE)
	$(CC) $(CFLAGS) -c $*.$(LANGUAGE)
