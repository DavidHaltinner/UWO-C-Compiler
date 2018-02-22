# Makefile for p4

CC     = g++
CFLAGS = -Wall -g

LEX    = flex
LEXLIB = -lfl
YACC   = bison -yd

OBJ    = main.o message.o symbol.o symboltable.o symlist.o ast.o \
         y.tab.o lex.yy.o  namecheck.o typecheck.o unparse.o int2str.o\
	 codegen.o emit.o

p5: $(OBJ)
	$(CC)  $(OBJ)  -o p5    $(LEXLIB)
wosym: main.o message.o symbol.o symboltable.o symlist.obj ast.o y.tab.o lex.yy.o namecheck.o typecheck.o unparse.o int2str.o
	$(CC) main.o message.o symbol.o symboltable.o symlist.obj ast.o y.tab.o lex.yy.o namecheck.o typecheck.o unparse.o int2str.o -o p4 $(LEXLIB)

main.o: main.cc ast.h symbol.h scanner.h
	$(CC) -c $(CFLAGS)   main.cc
message.o: message.cc message.h
	$(CC) -c $(CFLAGS)   message.cc
symbol.o: symbol.cc symbol.h
	$(CC) -c $(CFLAGS)   symbol.cc
symboltable.o: symboltable.cc symbol.h symboltable.h
	$(CC) -c $(CFLAGS)   symboltable.cc
symlist.o: symlist.cc symlist.h
	$(CC) -c $(CFLAGS)   symlist.cc
ast.o: ast.cc ast.h symbol.h
	$(CC) -c $(CFLAGS)   ast.cc
unparse.o: unparse.cc ast.h symbol.h y.tab.h
	$(CC) -c $(CFLAGS)   unparse.cc

int2str.o: int2str.cc int2str.h
	$(CC) -c $(CFLAGS) int2str.cc

namecheck.o: namecheck.cc ast.h symbol.h symboltable.h message.h \
             symlist.h
	$(CC) -c $(CFLAGS)   namecheck.cc
typecheck.o: typecheck.cc ast.h symbol.h symboltable.h message.h \
             symlist.h
	$(CC) -c $(CFLAGS)   typecheck.cc

codegen.o: codegen.cc ast.h emit.h scanner.h y.tab.h message.h symlist.h\
	   symboltable.h int2str.h 
	$(CC) -c $(CFLAGS)   codegen.cc

emit.o:	 emit.cc emit.h ast.h message.h int2str.h
	$(CC) -c $(CFLAGS)   emit.cc

y.tab.o: y.tab.c
	$(CC) -c $(CFLAGS)   y.tab.c
y.tab.h: y.tab.c
	touch             y.tab.h
y.tab.c: grammar.yacc ast.h symbol.h scanner.h
	$(YACC)         grammar.yacc

lex.yy.o: lex.yy.c ast.h symbol.h scanner.h message.h int2str.h y.tab.h
	$(CC) -c $(CFLAGS)   lex.yy.c  -DYY_NO_UNPUT
lex.yy.c: tokens.lex y.tab.h
	$(LEX)              tokens.lex

listings:
	enscript -2r -P halseyLinux \
	main.cc Makefile codegen.cc

test:
	Go
clean:
	/bin/rm -f *.o lex.yy.c y.tab.*
erase:
	/bin/rm -f *.o *.s lex.yy.c y.tab.*  p5
preClean:
	/bin/rm -f *.s
