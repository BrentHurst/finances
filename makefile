EXECUTABLE = finances.exe
SOURCE = $(wildcard *.cpp)
HEADER = $(wildcard *.h)
# OBJECTS = $(wildcard *.o)
OBJECTS = $(wildcard $(HOME)/include/sources/Date/*.o $(HOME)/include/sources/iogbh/*.o $(HOME)/include/sources/RomanNumerals/*.o ./*.o)

CC = g++
LANGUAGE = cpp
INCLUDES = #-lDate -liogbh
INCLUDEDIR = #$(HOME)/include
LIBS = #$(INCLUDEDIR)/libDate.a $(INCLUDEDIR)/libiogbh.a
CFLAGS = -g -Wall #$(INCLUDES:%=-I%)
LFLAGS = -g $(INCLUDES)

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE) $(HEADER) *.o
	$(CC) $(LFLAGS) $(LIBS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.$(LANGUAGE)
	$(CC) $(CFLAGS) -c $*.$(LANGUAGE)

.PHONY: clean
clean:
	rm *.o
