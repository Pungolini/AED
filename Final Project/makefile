## Use EXE to represent the name of the program that we're building.
EXE2 = walker

## SRC is the list of .c files we need to build our program..
SRC = main.c utils.c board_process.c

CC              = gcc
CC_OPTIONS      = -O3 -Wall -std=c99

## Define OBJ, which symbolizes all the .o files that the compiler needs
##      to create and then link together in order to create the program.
bin:
	$(CC) $(SRC) $(CC_OPTIONS) -o  $(EXE2)
	

FILES = $(shell ls *.puz)



# tests
all: w s m
	
w:
	for F in ${FILES}; do  ./$(EXE2) $${F} ; done;

s:
	./$(EXE2) singlemap.puz

m:
	./$(EXE2) multiplemap.puz




# Run gdb
gdb:
	for F in ${FILES}; do  gdb -ex=r --args ./$(EXE2) $${F} ; done;

gdbs:
	gdb -ex=r --args ./$(EXE2) singlemap.puz

gdbm:
	gdb -ex=r --args ./$(EXE2) multiplemap.puz


A:
	for F in $(shell ls testes/A/*.puz); do  ./$(EXE2) $${F} ; done;

B:
	for F in $(shell ls testes/B/*.puz); do  ./$(EXE2) $${F} ; done;

C:
	for F in $(shell ls testes/C/*.puz); do  ./$(EXE2) $${F} ; done;

D:
	for F in $(shell ls testes/D/*.puz); do  ./$(EXE2) $${F} ; done;

E:
	for F in $(shell ls testes/E/*.puz); do  ./$(EXE2) $${F} ; done;

F:
	for F in $(shell ls testes/F/*.puz); do  ./$(EXE2) $${F} ; done;

Others:
	for F in $(shell ls testes/Others/*.puz); do  ./$(EXE2) $${F} ; done;

vlgs:
	valgrind --leak-check=full --track-origins=yes ./$(EXE2) singlemap.puz

# Run valgrind
vlg:
	for F in ${FILES}; do  valgrind --leak-check=full --track-origins=yes ./$(EXE2) $${F} ; done;

clean:
	rm -f $(EXE2) *.o a.out core


depend:         Makefile $(SRC)
	makedepend $(INCLUDES) $(SRC)




## end of Makefile
# DO NOT DELETE THIS LINE 
