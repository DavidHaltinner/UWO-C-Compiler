// symboltable.h

#ifndef _SYMBOLTABLE_GUARD
#define _SYMBOLTABLE_GUARD

#include <string>
#include "symbol.h"
#include <list>
#include <iostream>

// **********************************************************************
// The SymbolTable class implements a symbol table, which is a set of
// pointers to Symbols.
// **********************************************************************
//
// public member functions
// =======================
//
// constructor/destructor
// ======================
// not needed -- the defaults work fine
//
// mutators/modifiers
// ==================
// void Insert(SymbolPtr sym) -- add sym to the table (error if there is
//                            -- already a symbol with the same name)
//
// other operations
// ================
// SymbolPtr Lookup(string k)   -- if there is a symbol with the given name,
//                              -- return a pointer to it;
//                              -- otherwise, return NULL
// Print(ostream & out)
//                              -- print the names of all the symbols, one
//                              -- per line, to the given output stream
                                               
class SymbolTable
{
  public:
    // mutator/modifier member functions

    void Insert(SymbolPtr sym);

    // other operations

    SymbolPtr Lookup(string k);
    void Print(ostream &out);

  private:
    static const int TABLESIZE = 17;
    list<SymbolPtr> bucket[TABLESIZE];
    int Hash(string & S);
};

#endif
