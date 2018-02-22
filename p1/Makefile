p1: symbol.o symboltable.o main.o
	g++ -o p1 symboltable.o symbol.o main.o
p1.o: main.cc
	g++ -g -c main.cc
symboltable.o: symboltable.cc symboltable.h
	g++ -g -c symboltable.cc
symbol.o: symbol.cc symbol.h
	g++ -g -c symbol.cc
clean:
	rm -rf *.o
	rm -rf core
	rm -rf p1
