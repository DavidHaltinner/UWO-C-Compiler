# Makefile for p3

CC     = g++
CFLAGS = -Wall -g

LEX    = flex
LEXLIB = -lfl
YACC   = bison -yd

OBJ    = main.o message.o int2str.o ast.o unparse.o y.tab.o lex.yy.o

p3: $(OBJ)
	$(CC)  $(OBJ)  -o p3   $(LEXLIB)

test: p3 test.z
	p3 -u test.z >test.un


main.o: main.cc ast.h symbol.h scanner.h
	$(CC) -c $(CFLAGS)   main.cc
ast.o: ast.cc ast.h symbol.h
	$(CC) -c $(CFLAGS)   ast.cc
unparse.o: unparse.cc ast.h symbol.h y.tab.h
	$(CC) -c $(CFLAGS)   unparse.cc
message.o: message.cc message.h
	$(CC) -c $(CFLAGS)   message.cc
int2str.o: int2str.cc int2str.h
	$(CC) -c $(CFLAGS)   int2str.cc

y.tab.o: y.tab.c
	$(CC) -c $(CFLAGS)   y.tab.c
y.tab.h: y.tab.c
	touch y.tab.h
y.tab.c: grammar.yacc ast.h symbol.h scanner.h
	$(YACC)         grammar.yacc

lex.yy.o: lex.yy.c ast.h symbol.h scanner.h message.h int2str.h y.tab.h
	$(CC) -c $(CFLAGS)   lex.yy.c  -DYY_NO_UNPUT
lex.yy.c: tokens.lex y.tab.h
	$(LEX) tokens.lex


#test.un is the unparsing of the program test.z
listings:
	enscript -2r main.cc test.z test.un grammar.yacc unparse.cc

clean:
	/bin/rm -f *.o lex.yy.c y.tab.*
erase:
	/bin/rm -f *.o lex.yy.c y.tab.*  p3
