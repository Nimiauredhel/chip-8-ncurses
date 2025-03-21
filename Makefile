PROGRAM=program
FLAGS= -std=c99 -Wall -pedantic -Wextra -fshort-enums
ARGS=
INC=-lncurses -lportaudio

default:
	gcc *.c $(INC) $(FLAGS) -o $(PROGRAM).o
                                     
strict:                              
	bear -- gcc *.c $(INC) $(FLAGS) -o $(PROGRAM).o
                    
debug:              
	gcc *.c $(INC) $(FLAGS) -g -o $(PROGRAM).o

run:
	./$(PROGRAM).o $(ARGS)

andrun:
	gcc *.c -o $(PROGRAM).o
	./$(PROGRAM).o $(ARGS)

gdb:
	gdb ./$(PROGRAM).o $(ARGS)

valgrind:
	valgrind -s --leak-check=yes --track-origins=yes ./$(PROGRAM).o $(ARGS)

clean:
	rm -f $(PROGRAM).o
	rm -f compile_commands.json

