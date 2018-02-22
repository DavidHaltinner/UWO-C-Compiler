## Makefile for Program 2

CC     = g++
CFLAGS = -Wall -g

LEX    = flex
LEXLIB = -lfl
OBJ    = main.o message.o lex.yy.o

p2: $(OBJ)
	$(CC) $(OBJ)  -o p2    $(LEXLIB)


main.o: main.cc scanner.h
	$(CC) -c $(CFLAGS)   main.cc
message.o: message.cc message.h
	$(CC) -c $(CFLAGS)   message.cc

lex.yy.o: lex.yy.c scanner.h message.h y.tab.h
	$(CC) -c $(CFLAGS)   lex.yy.c   -DYY_NO_UNPUT
lex.yy.c: tokens.lex
	$(LEX) tokens.lex

clean:
	/bin/rm -f  *.o  lex.yy.c
