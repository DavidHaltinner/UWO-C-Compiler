# Makefile for p4

CC     = g++
CFLAGS = -Wall -g

LEX    = flex
LEXLIB = -lfl
YACC   = bison -yd

OBJ    = main.o message.o symbol.o symboltable.o symlist.o ast.o \
         y.tab.o lex.yy.o  namecheck.o typecheck.o unparse.o int2str.o

p4: $(OBJ)
	$(CC)  $(OBJ)  -o p4    $(LEXLIB)
wosym: main.o message.o symbol.o symboltable.o symlist.obj ast.o y.tab.o lex.yy.o namecheck.o typecheck.o unparse.o int2str.o
	$(CC) main.o message.o symbol.o symboltable.o symlist.obj ast.o y.tab.o lex.yy.o namecheck.o typecheck.o unparse.o int2str.o -o p4 $(LEXLIB)

test: p4
	-@echo ""
	-@echo "==========================================="
	-./p4 b1.z
	-@echo "==========================================="
	-./p4 -u g.z
	-@echo "==========================================="
	-@echo ""


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
	main.cc g.z g.un unparse.cc namecheck.cc typecheck.cc

clean:
	/bin/rm -f *.o lex.yy.c y.tab.*
erase:
	/bin/rm -f *.o lex.yy.c y.tab.*  p4
