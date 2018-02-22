// symbol.h

#ifndef SYMBOL_GUARD
#define SYMBOL_GUARD

#include <string>
using namespace std;


// *********************************************
// The Symbol class defines a symbol-table entry
// *********************************************
//
// Member functions
// ================
//
// constructor
// ===========
// Symbol(string S) -- constructor: creates a Symbol with the given name
//

enum SymbolType {VoidType, IntType, BoolType, StringType, ErrorType};
enum SymbolKind {VarKind,ArrayKind, FnKind, ValParamKind,ArrayParamKind, ErrorKind};
enum SymbolAdr  {Global, Local};

class Symbol {
  public:
    Symbol(string S);   // constructor

    string symbolName;  // symbol name

    //set by the name checker

    SymbolType symbolType;
    SymbolKind symbolKind;

    int arraySize;
    int offset;
    int parameterCount;  // only for functions
    int localVarCount;   // only for functions
    int frameSize;       // only for functions

    //set by the code generator

    SymbolAdr  adr;

    // class function (for debugging)
    static string printSymbolType( SymbolType t );
};

typedef Symbol * SymbolPtr;

#endif
