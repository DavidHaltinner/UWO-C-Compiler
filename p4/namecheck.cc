// namecheck.cc

/*
 *  build symbol table and check for declaration/use errors:
 *      a) multiply declared identifiers
 *      b) undeclared identifiers
 *  find errors with the "main" function
 *  compute offsets and other information
 */
using namespace std;
#include <string>
#include "symbol.h"
#include "symboltable.h"
#include "symlist.h"
#include "ast.h"
#include "message.h"

#define DEBUG 0
#define BUG(d) if(d)cerr<<whoami()<<endl;cerr.flush();


static SymTableList  symList;
static SymbolTable * ptrCurrentSymTab;

int declErrors        = 0;
int currentOffset     = 0;
int numParameters     = 0;
int localVarCount     = 0;
bool isGlobal         = true;

void ProgramNode :: checkNames()
{
  // Global Symbol Table
  symList.push( new SymbolTable );
  ptrCurrentSymTab = symList.top();
  // ... your code goes here
  myGlobalList->checkNames();
  isGlobal = false;
  myFunctionList->checkNames();
  string str = "main";
  if( symList.lookUp(str) == NULL || symList.lookUp(str)->symbolKind != FnKind){
	string msg = "function main is missing";
	Error(0,0,msg);
	declErrors++;
  }
  symList.pop();
}


void GlobalListNode :: checkNames()
{
  myGlobalList -> checkNames();
  myDecl       -> checkNames();
}

void FunctionListNode :: checkNames()
{
  myFunctionList -> checkNames();
  myFunctionDecl -> checkNames();
}
// in file namecheck.cc, these are the only two nodes that
// will record a data type in the member nodeType
// this is done so that symbols entered in the symbol table
// will have complete information
// the next pass, implemented in file typecheck.cc, will
// give other nodeType members data type information

void TypeIntNode  :: checkNames()  { nodeType =  IntType; }
void TypeBoolNode :: checkNames()  { nodeType = BoolType; }


void VariableDeclNode :: checkNames()    // int x;
{
  myType -> checkNames();  // data type
  localVarCount++;

  // name is being DECLARED
  // check for multiply-declared variable name

  // strategy
  // we cannot recurse to the identifier node because
  //   identifier node assumes the USAGE of a name
  // so, we must access identifier node information indirectly
  //   by following the pointer to the identifier node

  string  varName = myIdentifier -> getMyStringVal();   // variable name
  Symbol * symbol = ptrCurrentSymTab -> Lookup(varName);  // look it up

  if (symbol != NULL) {
	// name is already in the symbol table
	// ERROR : variable is MULTIPLY_DECLARED
	string msg =  varName + " is already declared";
	Error(line,column,msg);
	declErrors++;
  }
  else{
	// create symbol
	Symbol * s = new Symbol(varName);
	s->symbolType = myType->nodeType;
	s->symbolKind = VarKind;
	s->offset = currentOffset;
	if(isGlobal){
		s->adr = Global;
	}
	else{
		s->adr = Local;
	}

	currentOffset += 4;
	s->arraySize = 0;
	ptrCurrentSymTab->Insert(s);
	myIdentifier->setMySymbolTableEntry(s);
  }
}

void ArrayDeclNode :: checkNames()
{
  myType -> checkNames();

  string varName =  myIdentifier -> getMyStringVal();
  Symbol * symbol = ptrCurrentSymTab -> Lookup(varName);

  if (symbol != NULL) {
	// name is already in the symbol table
	// ERROR : variable is MULTIPLY_DECLARED
	string msg =  varName + " is already declared";
	Error(line,column,msg);
	declErrors++;
  }
  else{
	// create symbol
	Symbol * s = new Symbol(varName);
	s->symbolType = myType-> nodeType;
	s->symbolKind = ArrayKind;
	s->arraySize = myIntLit->myIntVal;
	if(isGlobal){
		s->adr = Global;
	}
	else{
		s->adr = Local;
	}
	localVarCount += s->arraySize;
	currentOffset += s->arraySize * 4;
	s->offset = currentOffset - 4;
	ptrCurrentSymTab->Insert(s);
	myIdentifier->setMySymbolTableEntry(s);
  }
}

void FunctionDeclNode :: checkNames()
{
  bool multipleDecl = false;
  currentOffset  = 0;
  numParameters  = 0;
  localVarCount  = 0;

  myType -> checkNames();
 
  // your code goes here
  string  varName = myIdentifier -> getMyStringVal();   // variable name
  Symbol * symbol = ptrCurrentSymTab -> Lookup(varName);  // look it up
  
  if (varName == "main") {
	if(myType->isNull() || myType->nodeType == BoolType){
		string msg = varName + " does not return int";
		Error(line,column,msg);
		declErrors++;
	}
  }

  if (symbol != NULL) {
	// name is already in the symbol table
        // ERROR : function is MULTIPLY_DECLARED	
	multipleDecl = true;
	string msg =  varName + " is already declared";
	Error(line,column,msg);
	declErrors++;
  }
  // create symbol
  mySymbolTableEntry = new Symbol(varName);

  if(!multipleDecl) ptrCurrentSymTab->Insert(mySymbolTableEntry);
  // make new Symbol table and set it as the current one
  symList.push(new SymbolTable);
  ptrCurrentSymTab = symList.top();

  myParameters -> checkNames();
  currentOffset += 8;

  if(numParameters != 0 && varName == "main"){
	string msg = varName + " has a parameter";
	Error(line, column, msg);
	declErrors++;
  }
  // in either case, keep going; name check body of function
  myVarList  -> checkNames();
  myStmtList -> checkNames();

  symList.pop();
  ptrCurrentSymTab = symList.top();

  // if not multiply declared
  // insert values into Symbol then put it into the symbolTable
  if(! multipleDecl ){
  	if(!myType->isNull()){
		mySymbolTableEntry->symbolType = myType->nodeType;
  	}
 	else{
		mySymbolTableEntry->symbolType = VoidType;	
  	}
	mySymbolTableEntry->symbolKind = FnKind;
  	mySymbolTableEntry->parameterCount = numParameters;
	mySymbolTableEntry->frameSize      = (numParameters + localVarCount + 2) * 4;
  	mySymbolTableEntry->localVarCount  = localVarCount;
  	//ptrCurrentSymTab->Insert(mySymbolTableEntry);
  }
}

void ParametersNode :: checkNames()
{
  myParamList->checkNames();
}

void ParamListNode :: checkNames()
{
  myParamList->checkNames();
  myParam    ->checkNames();
}

void ArgListNode :: checkNames()
{
  myArgList   ->checkNames();
  myExpression->checkNames();	
}

void ParamValNode :: checkNames() 
{
  myType -> checkNames();
  numParameters++;

  string  varName = myIdentifier -> getMyStringVal();   // variable name
  Symbol * symbol = ptrCurrentSymTab -> Lookup(varName);  // look it up

  if (symbol != NULL) {
	// name is already in the symbol table
	// ERROR : variable is MULTIPLY_DECLARED
	string msg =  varName + " is already declared";
	Error(line,column,msg);
	declErrors++;
  }
  else{
	// create symbol
	Symbol * s = new Symbol(varName);
	s->symbolType = myType -> nodeType;
	s->symbolKind = ValParamKind;
	s->offset = currentOffset;
	currentOffset += 4;
	s->arraySize = 0;
	ptrCurrentSymTab->Insert(s);
	myIdentifier->setMySymbolTableEntry(s);
  }
}

void ParamArrayNode :: checkNames()
{
  myType -> checkNames();
  numParameters++;
  
  string  varName = myIdentifier -> getMyStringVal();   // variable name
  Symbol * symbol = ptrCurrentSymTab -> Lookup(varName);  // look it up

  if (symbol != NULL) {
	// name is already in the symbol table
	// ERROR : variable is MULTIPLY_DECLARED
	string msg =  varName + " is already declared";
	Error(line,column,msg);
	declErrors++;
  }
  else{
	// create symbol
	Symbol * s = new Symbol(varName);
	s->symbolType = myType -> nodeType;
	s->symbolKind = ArrayParamKind;
	s->offset = currentOffset;
	currentOffset += 4;
	ptrCurrentSymTab->Insert(s);
	myIdentifier->setMySymbolTableEntry(s);
  }
}

void VarListNode :: checkNames()
{
  myVarList->checkNames();
  myDecl   ->checkNames();
}

void StmtListNode :: checkNames() 
{
  myStmtList -> checkNames();
  myStmt     -> checkNames();
}

void AssignNode :: checkNames()
{
  myTarget     -> checkNames();
  myExpression -> checkNames();
}

void IfNode :: checkNames()
{
  myExpression-> checkNames();
  myThenStmt  -> checkNames();
  myElseStmt  -> checkNames();
}

void WhileNode :: checkNames()
{
  myExpression -> checkNames();
  myStmt       -> checkNames();
}

void BlockNode :: checkNames()
{
  // make new Symbol table and set it as the current one
  symList.push(new SymbolTable);
  ptrCurrentSymTab = symList.top();

  // check the children's names
  myVarList  -> checkNames();
  myStmtList -> checkNames();
  
  // now that you're leaving take the symboltable off the stack
  symList.pop();
  ptrCurrentSymTab = symList.top();
}

void ReturnNode :: checkNames() 
{
  myReturnVal->checkNames();
}

void ProcCallNode :: checkNames()
{
  myIdentifier -> checkNames();
  myArgList    -> checkNames();
}

void PrintNode :: checkNames()
{
  myPrintList -> checkNames();
}

void PrintListNode :: checkNames()
{
  myPrintList-> checkNames();
  myPrintItem-> checkNames();
}

void OrNode :: checkNames()
{
  myLeft  -> checkNames();
  myRight -> checkNames();
}

void AndNode :: checkNames()
{
  myLeft  -> checkNames();
  myRight -> checkNames();
}

void RelOpNode :: checkNames()
{
  myLeft  -> checkNames();
  myRight -> checkNames();
}

void ArithOpNode :: checkNames()
{
  myLeft  -> checkNames();
  myRight -> checkNames();
}

void TrueNode :: checkNames() 
{
  return;
}

void FalseNode :: checkNames()
{
  return;
}

void NameVarNode :: checkNames()
{
  myIdentifier-> checkNames();
}

void ArrayVarNode :: checkNames()
{
  myIdentifier->checkNames();
  myExpression->checkNames();
}

void FnCallNode :: checkNames()
{
  myIdentifier->checkNames();
  myArgList   ->checkNames();
}

void IntLiteralNode :: checkNames()
{
  return;
}

void TargetNode :: checkNames()
{
  myIdentifier->checkNames();
  myExpression->checkNames();
}

void StringLiteralNode :: checkNames()
{
  return;
}

void IdentifierNode :: checkNames()   // name USAGE
{
  // getting here recursively means that a
  // name is being USED; check for undefined name
  string varName = getMyStringVal();
  Symbol * symbol = symList.lookUp(varName); 

  if (symbol == NULL){
	string msg = varName + " is undeclared";
	Error(this->line, this->column, msg);
	declErrors ++;
	symbol = new Symbol(varName);
	symbol->symbolType = ErrorType;
  	ptrCurrentSymTab -> Insert(symbol);
  }
  setMySymbolTableEntry(symbol);
}
