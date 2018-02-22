
// symboltable.cc

#include <cassert>
#include <list>
#include <string>
#include <iostream>
#include <iomanip>
#include "symbol.h"
#include "symboltable.h"

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
	
	// Hash the item
	int hashVal = Hash((sym->symbolName));

	// Put it in the back of the bucket	
	bucket[hashVal].push_back(sym);
}

// ======================================================================
// Lookup
// ======================================================================
// if there is a symbol with the given name in the table, return a
//    pointer to that symbol;
// otherwise, return NULL

SymbolPtr SymbolTable::Lookup(string name)
{
	int hashVal = Hash(name);	// Which list to look in
	list<SymbolPtr> L;		// Pointer to the list
	list<SymbolPtr>::iterator i;	// Iterator for the list
	SymbolPtr sym;			// Pointer for examining items in list
	bool found = false;		// Tells if item is found or not
	
	L = bucket[hashVal];
	i = L.begin();	
 
	while( i != L.end()){
		sym = *i;
				
		if(sym->symbolName == name){
			found = true;	// The item is found
			return sym;	// Now stop looking
		}
	
		i++;
	}		

	return NULL;
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
