// symbol.h

#ifndef SYMBOL_GUARD
#define SYMBOL_GUARD

#include <string>
using namespace std;

// **********************************************************************
// The Symbol class defines a symbol-table entry;
// an object that contains a name (a string)
// **********************************************************************
//
// Member functions
// ================
//
// constructor
// ===========
// Symbol(string S) -- constructor: creates a Symbol with the given name
//

enum SymbolType {IntType, VoidType, ErrorType};
enum SymbolKind {VarKind, FnKind,   ErrorKind};


class Symbol {
  public:
    Symbol(string S);   // constructor

    string symbolName;  // symbol name

    SymbolType symbolType;  // not used for p1
    SymbolKind symbolKind;  // not used for p1
};

typedef Symbol * SymbolPtr;

#endif
