EXEC = a.out
CC = g++
OBJ = lexer.o main.o parser.o

$(EXEC) : $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) -lm

lexer.o : lexer.cpp lexer.h
	$(CC) -c -g lexer.cpp

main.o : main.cpp lexer.h
	$(CC) -c -g main.cpp

parser.o:parser.cpp parser.h
	$(CC) -c -g parser.cpp

clean:
	rm -f $(EXEC) lexer.o main.o parser.o
