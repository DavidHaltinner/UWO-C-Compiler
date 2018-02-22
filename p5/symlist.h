// symlist.h

#ifndef SYMLIST_H_
#define SYMLIST_H_

#include <list>
#include <string>

#include "symboltable.h"
using namespace std;

typedef SymbolTable *PSymbolTable;


class SymTableList {

public:
	 SymTableList();
	~SymTableList();

	void         push( PSymbolTable v );  // AddToFront
	PSymbolTable pop();                   // RemoveFromFront
	PSymbolTable top();                   // current scope

	SymbolPtr    lookUp( string id );     // is id in any scope?

private:
	list<PSymbolTable> * symTableList;    // STL list
};

#endif
