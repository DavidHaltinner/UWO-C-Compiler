// typecheck.cc
#include "ast.h"
#include "message.h"
#include "symbol.h"
#include "symboltable.h"
#include "symlist.h"
#include "scanner.h"
#include "y.tab.h"

/*
 *  do type checking, i.e. find type errors
 *
 */


int typeErrors = 0;


void ProgramNode :: checkTypes()
{
  myFunctionList -> checkTypes();
}

void GlobalListNode :: checkTypes()
{
  return;
}

void FunctionListNode :: checkTypes()
{
  myFunctionList -> checkTypes();
  myFunctionDecl -> checkTypes();
}


void FunctionDeclNode :: checkTypes()
{
  myStmtList -> checkTypes();
}

void VariableDeclNode :: checkTypes()
{
  return;
}

void ArrayDeclNode :: checkTypes()
{
  return;
}

void TypeIntNode :: checkTypes()
{
  return;
}

void TypeBoolNode :: checkTypes()
{
  return;
}

void ParametersNode :: checkTypes()
{
   return;
}

void ParamListNode :: checkTypes()
{
   return;
}

void ArgListNode :: checkTypes()
{
   myArgList -> checkTypes();
   myExpression -> checkTypes();
}

void ParamValNode :: checkTypes()
{
  return;
}

void ParamArrayNode :: checkTypes()
{
  return;
}

void VarListNode :: checkTypes()
{
  myVarList -> checkTypes();
  myDecl -> checkTypes();
}

void StmtListNode :: checkTypes()
{
  myStmtList -> checkTypes();
  myStmt -> checkTypes();
}

void AssignNode :: checkTypes()
{
  bool errors = false;
  myTarget -> checkTypes();
  myExpression -> checkTypes();
  Symbol * s = myTarget->getID()->getSymbol();
  if((s->symbolKind == ArrayKind || s->symbolKind == ArrayParamKind)
	&& myTarget->getExp()->isNull()){
	typeErrors++;
	string msg = "assignment: LHS name is an array name";
	Error(line, column, msg);
	errors = true;
  }
  if(s->symbolKind == FnKind){
	typeErrors++;
	string msg = "assignment: LHS name is a function name";
	Error(line, column, msg);
	errors = true;
  }
  if(myExpression->isName()){
	s = myExpression->getID()->getSymbol();
  	if(s->symbolKind == FnKind){
		typeErrors++;
		string msg = "assignment: RHS name is a function name";
		Error(line, column, msg);
		errors = true;
  	}
  	else if(s->symbolKind == ArrayKind || s->symbolKind == ArrayParamKind){
		typeErrors++;
		string msg = "assignment: RHS name is an array name";
		Error(line, column, msg);
		errors = true;
  	}
  }
  if(myExpression-> nodeType != myTarget->nodeType && !errors){
	typeErrors++;
	string msg = "assignment: LHS type != RHS type";
	Error(line, column, msg);
  } 
}

void IfNode :: checkTypes()
{
  myExpression -> checkTypes();
  myThenStmt -> checkTypes();
  myElseStmt -> checkTypes();
}

void WhileNode :: checkTypes()
{
  myExpression -> checkTypes();
  myStmt -> checkTypes();
}

void BlockNode :: checkTypes()
{
  myStmtList -> checkTypes();
  return;
}

void ReturnNode :: checkTypes()
{
  myReturnVal -> checkTypes();
  nodeType = myReturnVal -> nodeType;
}

void ProcCallNode :: checkTypes()
{
  myArgList->checkTypes();
}

void PrintNode :: checkTypes()
{
  myPrintList -> checkTypes();
}

void PrintListNode :: checkTypes()
{
  myPrintList -> checkTypes();
  myPrintItem -> checkTypes();
  if(myPrintItem->nodeType == VoidType){
	typeErrors++;
	myPrintItem->nodeType = ErrorType;
	string msg = "print expression is void type";
	Error(line, column, msg);
  }
}

void OrNode :: checkTypes()
{
  myLeft->checkTypes();
  myRight->checkTypes();
  nodeType = myRight -> nodeType;
  if(!myLeft->isNull() && !myRight->isNull()){
	if(myLeft->nodeType != BoolType){
		typeErrors++;
		string msg = "left  operand of || is not bool";
		Error(line, column, msg);
  	}
	if(myRight->nodeType != BoolType){
		typeErrors++;
		string msg = "right operand of || is not bool";
		Error(line, column, msg);
  	}
	nodeType = BoolType;
  }
}

void AndNode :: checkTypes()
{
  myLeft->checkTypes();
  myRight->checkTypes();
  nodeType = myRight->nodeType;
  if(!myLeft->isNull()&&!myRight->isNull()){
	if(myLeft->nodeType != BoolType){
		typeErrors++;
		string msg = "left  operand of && is not bool";
		Error(line, column, msg);
  	}

	if(myRight->nodeType != BoolType){
		typeErrors++;
		string msg = "right operand of && is not bool";
		Error(line, column, msg);
  	}
	nodeType = BoolType;
  }
}

void RelOpNode :: checkTypes()
{
  myLeft->checkTypes();
  myRight->checkTypes();
  nodeType = myRight->nodeType;
  if(!myLeft->isNull()&&!myRight->isNull()){
	if(myLeft->nodeType != IntType){
		typeErrors++;
		string msg = "left  operand of"; 
		switch(myRelOp){
			case LT:
				msg = msg + "  < ";
				break;
			case GT:
				msg = msg + "  > ";
				break;
			case LEQ:
				msg = msg + " <= ";
				break;
			case GEQ: 
				msg = msg + " >= ";
				break;
			case EQ:
				msg = msg + " == ";
				break;
			case NEQ: 
				msg = msg + " != ";
				break;
		}
		msg = msg + "is not int";
		Error(line, column, msg);
  	}
	if(myRight->nodeType != IntType){
		typeErrors++;
		string msg = "right operand of"; 
		switch(myRelOp){
			case LT:
				msg = msg + "  < ";
				break;
			case GT:
				msg = msg + "  > ";
				break;
			case LEQ:
				msg = msg + " <= ";
				break;
			case GEQ: 
				msg = msg + " >= ";
				break;
			case EQ:
				msg = msg + " == ";
				break;
			case NEQ: 
				msg = msg + " != ";
				break;
		}
		msg = msg + "is not int";
		Error(line, column, msg);
	}
	nodeType = BoolType;
  }
}

void ArithOpNode :: checkTypes()
{
  myLeft->checkTypes();
  myRight->checkTypes();
  nodeType = myRight->nodeType;
  if(!myLeft->isNull()&&!myRight->isNull()){
  	if(myLeft->nodeType != IntType){
		typeErrors++;
		string msg = "left  operand of ";
		switch(myArithOp){
			case PLUS:
				msg = msg + " + ";
				break;
			case MINUS:
				msg = msg + " - ";
				break;
			case TIMES:
				msg = msg + " * ";
				break;
			case SLASH: 
				msg = msg + " / ";
				break;
		}
		msg = msg + "is not int";
		Error(line, column, msg);
  	}
  	if(myRight->nodeType != IntType){
		typeErrors++;
		string msg = "right operand of ";
		switch(myArithOp){
			case PLUS:
				msg = msg + " + ";
				break;
			case MINUS:
				msg = msg + " - ";
				break;
			case TIMES:
				msg = msg + " * ";
				break;
			case SLASH: 
				msg = msg + " / ";
				break;
		}
		msg = msg + "is not int";
		Error(line, column, msg);
  	}
	nodeType = IntType;
  }
}

void NameVarNode :: checkTypes()
{
  myIdentifier -> checkTypes();
  nodeType = myIdentifier->nodeType;
}

void ArrayVarNode :: checkTypes()
{
  myIdentifier -> checkTypes();
  nodeType = myIdentifier->nodeType;

  myExpression -> checkTypes(); 
}

void FnCallNode :: checkTypes()
{
  myIdentifier->checkTypes();
  nodeType = (myIdentifier->getSymbol())->symbolType;
  myArgList->checkTypes();
}

void TrueNode :: checkTypes() { nodeType = BoolType; }
void FalseNode :: checkTypes() { nodeType = BoolType; }
void IntLiteralNode :: checkTypes() { nodeType = IntType; }
void TargetNode :: checkTypes()
{
  Symbol * s = myIdentifier->getSymbol();
  nodeType = s->symbolType;
}
void IdentifierNode :: checkTypes() { nodeType = mySymbolTableEntry->symbolType; }
void StringLiteralNode :: checkTypes() { nodeType = StringType; }
