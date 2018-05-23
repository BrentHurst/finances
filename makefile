THESEOBJECTS =  account.o \
				ask.o \
				cmdcomp.o \
				cmd.o \
				finances.o \
				link.o \
				load.o \
				macro.o \
				main.o \
				merge.o \
				number.o \
				printfinances.o \
				printtra.o \
				readaccount.o \
				readtra.o \
				readtype.o \
				reconcile.o \
				reconcilefinances.o \
				save.o \
				selectaccount.o \
				selecttransaction.o \
				selecttransfer.o \
				setup.o \
				tracomp.o \
				transaction.o \
				transfer.o

EXTERNALLIBOBJECTS =    ExternalLibObjects/ConstrDestr.o \
						ExternalLibObjects/getDateNothing.o \
						ExternalLibObjects/getMemberVar.o \
						ExternalLibObjects/iogbhDelimitedLineFile.o \
						ExternalLibObjects/operators.o \
						ExternalLibObjects/printDate.o \
						ExternalLibObjects/read.o \
						ExternalLibObjects/setProtected.o \
						ExternalLibObjects/setPublic.o

EXECUTABLE = finances.exe
MAINHEADER = finances.h
OBJECTS = $(THESEOBJECTS) $(EXTERNALLIBOBJECTS)
CC = g++
LANGUAGE = cpp
CFLAGS = -g -Wall 
LFLAGS = -g 


all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) -o $(EXECUTABLE) $(OBJECTS)

%.o: %.$(LANGUAGE) %.h $(MAINHEADER)
	$(CC) $(CFLAGS) -c $*.$(LANGUAGE)

.PHONY: clean
clean:
	rm $(THESEOBJECTS)
