EXECUTABLE = finances.exe
SOURCE = $(wildcard *.cpp)
HEADER = $(wildcard *.h)
OBJECTS = $(wildcard *.o)

CC = g++
LANGUAGE = cpp
INCLUDES = -lDate
LIBS = 
CFLAGS = -g -Wall $(INCLUDES) #$(INCLUDES:%=-I%)
LFLAGS = -g

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE) $(HEADER) *.o
	$(CC) $(LFLAGS) $(LIBS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.$(LANGUAGE)
	$(CC) $(CFLAGS) -c $*.$(LANGUAGE)

.PHONY: clean
clean:
	rm *.o
