// symlist.cc

#include "message.h"
#include "symlist.h"

SymTableList::SymTableList()
{
  symTableList = new list<PSymbolTable>();
}

SymTableList::~SymTableList()
{
  delete symTableList;
}

void SymTableList::push(PSymbolTable v) {
  symTableList->push_front(v);
}
PSymbolTable SymTableList::pop() {
  PSymbolTable p=symTableList->front();
  symTableList->pop_front();
  return p;
}
PSymbolTable SymTableList::top() {
  return symTableList->front();
}
SymbolPtr SymTableList::lookUp(string id) {
  list<PSymbolTable>::iterator i;
  for (i=symTableList->begin(); i != symTableList->end(); i++) {
    SymbolPtr p=(*i)->Lookup(id);
    if (p!=NULL) return p;
  }
  return NULL;
}
