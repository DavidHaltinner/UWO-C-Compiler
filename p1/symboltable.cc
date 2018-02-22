// symboltable.cc

#include <cassert>
#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include "symbol.h"
#include "symboltable.h"
using namespace std;

// implementation of the SymbolTable class using a hash table

/*		Assignment #1:
 *		 a) Do not modify functions Hash or Print
 *		 b) Implement functions Insert and Lookup
 */

// ======================================================================
// Insert
// ======================================================================
// add the given symbol to the table
// error if there is already a symbol with the same name

void SymbolTable::Insert(SymbolPtr sym)
{
  //error if Lookup(sym->symbolName)!=NULL
  assert(Lookup(sym->symbolName) == NULL);

  // at this point, Lookup(sym->symbolName) == NULL
  // i.e.  we have a guarantee that sym is not already in the symbol table
  // software needs to call Lookup before calling Insert
  // i.e.  duplicate names will never be in the symbol table

  // now, add sym to the symbol table
  int hashedValue=Hash(sym->symbolName); //Get the hash value of the word
  bucket[hashedValue].push_back(sym);	 //Drop it into the correct list
}

// ======================================================================
// Lookup
// ======================================================================
// if there is a symbol with the given name in the table, return a
//    pointer to that symbol;
// otherwise, return NULL

SymbolPtr SymbolTable::Lookup(string name)
{
  SymbolPtr sym;	//Point to the symbols im looking at
  list<SymbolPtr> L;	//My list pointer
  list<SymbolPtr>::iterator i; 	//the ierator for the list
  L = bucket[Hash(name)];	//Point the pointer at the correct list
  i = L.begin();		//Start the iterator at the front
  while ( i != L.end() ) {	//While i havent got to the end....
    sym = *i;			//make sym look at where the iterator is
    if (name == sym->symbolName) return sym; //Match: return the symbol
    i++;			//Next symbol...
  }
  return NULL;	//didnt find it, so return NULL.
}

// ======================================================================
// Hash
// ======================================================================
// hash function

int SymbolTable::Hash(string & S)
{
  int val = 0;
  for (int j=0; j<int(S.length()); j++) val = (val << 3) + S[j];
  if (val < 0) val = -val;
  return(val % TABLESIZE);
}

// ======================================================================
// Print
// ======================================================================
// print the names of all the symbols, one per line

void SymbolTable::Print(ostream & out)
{
  SymbolPtr       sym;
  list<SymbolPtr> L;
  list<SymbolPtr>::iterator i;

  for (int k=0; k<TABLESIZE; k++) {
    L = bucket[k];
    i = L.begin();
    while ( i != L.end() ) {
      sym = *i;
      out << sym->symbolName.c_str() << endl;
      i++;
    }
  }
}
