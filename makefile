THESEOBJECTS =  account.o \
				ask.o \
				cmdcomp.o \
				finances.o \
				foreign.o \
				global.o \
				link.o \
				load.o \
				macros.o \
				main.o \
				merge.o \
				print.o \
				reconcile.o \
				save.o \
				selectaccount.o \
				selecttransaction.o \
				selecttransfer.o \
				setup.o \
				tracomp.o \
				transaction.o \
				transfer.o

EXTERNALLIBOBJECTS =    ConstrDestr.o \
						getDateNothing.o \
						getMemberVar.o \
						iogbhDelimitedLineFile.o \
						operators.o \
						printDate.o \
						read.o \
						setProtected.o \
						setPublic.o

EXECUTABLE = finances.exe
HEADER = finances.h
OBJECTS = $(THESEOBJECTS) $(EXTERNALLIBOBJECTS)
CC = g++
LANGUAGE = cpp
CFLAGS = -g -Wall 
LFLAGS = -g 


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.$(LANGUAGE) $(HEADER)
	$(CC) $(CFLAGS) -c $*.$(LANGUAGE)

.PHONY: clean
clean:
	rm $(THESEOBJECTS)
