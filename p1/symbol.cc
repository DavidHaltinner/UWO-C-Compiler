// symbol.cc

#include "symbol.h"
#include <string>
using namespace std;
// implementation of Symbol class

// **********************************************************************
// Symbol constructor: initialize Symbol to have given name
// **********************************************************************
Symbol::Symbol(string S)
       : symbolName(S), symbolType(ErrorType), symbolKind(ErrorKind)
{}
