EXECUTABLE = finances.exe
HEADER = finances.h
THESEOBJECTS =  account.o \
				ask.o \
				finances.o \
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

OBJECTS =   ConstrDestr.o \
			getDateNothing.o \
			getMemberVar.o \
			iogbhDelimitedLineFile.o \
			operators.o \
			printDate.o \
			read.o \
			setProtected.o \
			setPublic.o \
			$(THESEOBJECTS)


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
