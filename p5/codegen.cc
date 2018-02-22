// codegen.cc             OO

using namespace std;
#include "ast.h"
#include "scanner.h"
#include "y.tab.h"
#include "message.h"
#include "symlist.h"
#include "symboltable.h"
#include "int2str.h"
#include "emit.h"
#include <list>
#include <map>

#define BUG(m)   // cerr<<m<<endl
#define EMIT(m)  // emitComment(m)

#define NUMERIC false // Turn on numeric style coding / turn off control flow

void emitPrintString();
void emitPrintInt ( int reg );
void emitPrintBool( int reg );

void emitL ( string label, string opcode="",
             string arg1="", string arg2="", string arg3="" );
void emitProgramHeader();
void emitProgramFooter();
void emitFunctionPrologue( string fn, int frameSize, int localVarCount );
void emitFunctionEpilogue( string fn, int parameterCount, int frameSize );
void declareGlobalVar  ( string name );
void declareGlobalArray( string name, int size );
void emitStringDefinition( string label, string String );

extern ofstream asmFile;
extern bool   SethiUllmanFlag;
static string    currentFunctionName("");
static SymbolPtr currentFunctionSym = NULL;
map<string, string> l;
//// static list<ExpressionNode *> * argumentList = NULL;
// ===========================================================================

string setRelOp( int kind )
{
  string s;

  switch (kind) {
     case LT : s = "slt"; break;
     case LEQ: s = "sle"; break;
     case GT : s = "sgt"; break;
     case GEQ: s = "sge"; break;
     case EQ : s = "seq"; break;
     case NEQ: s = "sne"; break;
     default : assert(0); break;
  }
  return s;
}

string binaryOp( int kind )
{
  string s;

  switch (kind) {
    case PLUS : s = "add" ; break;
    case MINUS: s = "sub" ; break;
    case TIMES: s = "mulo"; break;
    case SLASH: s = "div" ; break;
    default   : assert(0) ; break;
  }
  return s;
}

//*************storeIntoTarget is complete****************
void storeIntoTarget( ExpressionNode *astR, TargetNode *astL )
{
   int     reg  = astR->Reg;
   Symbol *ptrL = astL->getID()->getSymbol();

   if(ptrL->adr == Global && ptrL->symbolKind == VarKind){
	emit( "sw",  "$t"+IntToStr(reg),  ptrL->symbolName+".v" );
   }
   else if( ptrL->adr == Global && ptrL->symbolKind == ArrayKind){
	int regEx = astL->Reg;
	string regStr =  "0($t"+IntToStr(regEx)+")";
	emit("sw", "$t"+IntToStr(reg), regStr);
	freeReg(regEx);
   }
   else if( ptrL->adr == Local && 
	  ( ptrL->symbolKind == VarKind || ptrL->symbolKind == ValParamKind )){
	int o = ptrL->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	emit("sw", "$t"+IntToStr(reg), off+"($fp)");
   }
   else if( ptrL->adr == Local && ptrL->symbolKind == ArrayKind){
	int regEx = astL->Reg;
	int o = ptrL->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	string regStr = off+"($t"+IntToStr(regEx)+")";
	emit("sw", "$t"+IntToStr(reg), regStr);
	freeReg(regEx);
   }
   else if( ptrL->adr == Local && ptrL->symbolKind == ArrayParamKind){
	int regEx = astL->Reg;
	string regStr = "0($t"+IntToStr(regEx)+")";
	emit("sw", "$t"+IntToStr(reg), regStr);
	freeReg(regEx);
   }
}

void evaluateCondition( ExpressionNode *ast, string t, string f )
{
	if(ast->isAnd())	
	{
		string right(NextLabel());
		evaluateCondition(((AndNode *)ast)->getL(), right, f);
		emitLabel(right);
		evaluateCondition(((AndNode *)ast)->getR(), t, f);
	}
	else if(ast->isOr())
	{
		string next(NextLabel());
		evaluateCondition(((OrNode *)ast)->getL(), t, next);
		emitLabel(next);
		evaluateCondition(((OrNode *)ast)->getR(), t, f);
	}
	else if(ast->isRel()){ 
		string op;
		int kind = ((RelOpNode *)ast)->getOp();
		int leftReg  = ((RelOpNode *)ast)->emitLeft();
		int rightReg = ((RelOpNode *)ast)->emitRight();
		string left = "$t" + IntToStr(leftReg);
		string right = "$t" + IntToStr(rightReg);
		switch (kind) {
		     case LT : op = "blt"; break;
		     case LEQ: op = "ble"; break;
		     case GT : op = "bgt"; break;
		     case GEQ: op = "bge"; break;
		     case EQ : op = "beq"; break;
		     case NEQ: op = "bne"; break;
		     default : assert(0); break;
		}
		emit(op, left, right, t);
		emit("j", f);

		freeReg(leftReg);
		freeReg(rightReg);
	}
	else if(ast -> isTrue()){
		emit("j", t);
	}
	else if(ast -> isFalse()){
		emit("j", f);
	}
	else{
		ast->emitCode();
  		emit( "beqz", ast->Reg, f );
		emit( "j", t );
		freeReg(ast->Reg);
	}	
}
// ===========================================================================






void ProgramNode :: emitCode()
{
  emitProgramHeader();
  myGlobalList   -> emitCode();
  myFunctionList -> emitCode();
  emitProgramFooter();
}

void GlobalListNode :: emitCode()
{
  myGlobalList -> emitCode();
  myDecl       -> emitCode();
}

void FunctionListNode :: emitCode()
{
  myFunctionList -> emitCode();
  myFunctionDecl -> emitCode();
}

void ParametersNode :: emitCode()
{
  myParamList -> emitCode();
}

void ParamListNode :: emitCode()
{
  myParamList -> emitCode();
  myParam     -> emitCode();
}

void VarListNode:: emitCode()
{
  myVarList -> emitCode();
  myDecl    -> emitCode();
}

void StmtListNode :: emitCode()
{
  myStmtList -> emitCode();
  myStmt     -> emitCode();
}

void PrintNode :: emitCode()
{
  myPrintList -> emitCode();
}

void TypeIntNode  :: emitCode() {return;}
void TypeBoolNode :: emitCode() {return;}




void BlockNode :: emitCode()
{
  myVarList->emitCode();
  myStmtList->emitCode();
}

void ParamValNode :: emitCode()
{
	return;
}

void ParamArrayNode :: emitCode()
{
	return;
}

void ArgListNode :: emitCode()
{
	myArgList->emitCode();
	myExpression->emitCode();
	int reg = myExpression->Reg;
	emit("subu", "$sp", "$sp", IntToStr(4), "# argument");
	emit("sw", "$t"+IntToStr(reg), "($sp)");
	freeReg(reg);
}


// ===========================================================================

void VariableDeclNode :: emitCode()    // int x;
{
  //zinc always allocates 4 bytes for int and bool
  //so, do not need to visit myType

   if(myIdentifier->getSymbol()->adr == Global){
   	declareGlobalVar( myIdentifier->getMyStringVal()+".v" );
   }
}

void ArrayDeclNode :: emitCode()
{
  Symbol * s = myIdentifier->getSymbol();

  if(s->adr == Global){
	declareGlobalArray( s->symbolName+".v" , s->arraySize * 4 );
  } 
}

void FunctionDeclNode :: emitCode()
{
  currentFunctionName =  myIdentifier -> getMyStringVal();
  currentFunctionSym  = mySymbolTableEntry;
  emitFunctionPrologue( currentFunctionName,
			currentFunctionSym->frameSize,
			currentFunctionSym->localVarCount);

  myParameters -> emitCode();
  myVarList    -> emitCode();
  myStmtList   -> emitCode();

  emitFunctionEpilogue
     ( currentFunctionName,
       currentFunctionSym->frameSize,
       currentFunctionSym->parameterCount
     );

  if (currentFunctionName!="main")
	emit( "jr", "$ra" );      // return
  else{
	emitEmptyLine();
	emit( "li", "$v0", 10 );  // halt
	emit( "syscall", "\t\t# program exit" );
  }
}

// ===========================================================================

void PrintListNode :: emitCode()
{
  myPrintList -> emitCode();
  //emitComment( "print" );

  myPrintItem -> emitCode();   // string literal or expression

  switch( myPrintItem->nodeType ) {

     case StringType:
			emitPrintString();
			break;
     case IntType:
			emitPrintInt( myPrintItem->Reg );
			freeReg(myPrintItem->Reg);
			break;
     case BoolType:
			if(NUMERIC){
				emitPrintBool( myPrintItem->Reg );
				freeReg(myPrintItem->Reg);
			}
			else{
				string endLabel = NextLabel();	
  				emit( "la", "$a0", ".false" );
				emit( "beqz", myPrintItem->Reg, endLabel);
  				emit( "la", "$a0", ".true" );
  				emitLabel(endLabel);
  				emitPrintString();
				freeReg(myPrintItem->Reg);
			}
			break;
     default:
			assert(0);
			break;
  }
}


void AssignNode :: emitCode()
{
  emitComment( "assignment" );

  myExpression -> emitCode();  // translate RHS (expression)
  myTarget     -> emitCode();  // translate LHS (targetNode)

  storeIntoTarget( myExpression, myTarget );

  freeReg( myExpression->Reg );
  freeReg( myTarget    ->Reg );
}

void IfNode :: emitCode()
{
  emitComment("if");
  string trueLabel ( NextLabel() );
  string falseLabel( NextLabel() );
  string endifLabel( NextLabel() );
  
  if(NUMERIC){
  	myExpression -> emitCode();
  	emit( "beqz", myExpression->Reg, falseLabel );
  	freeReg(myExpression->Reg);
  }
  else{
	evaluateCondition(myExpression, trueLabel, falseLabel);
  }

  emitLabel(trueLabel);
  emitComment("then");
  myThenStmt -> emitCode();   // then clause
  emit( "j", endifLabel );

  emitLabel(falseLabel);
  emitComment("else");
  myElseStmt -> emitCode();   // else clause

  emitLabel(endifLabel);
}

void WhileNode :: emitCode()
{
  if(NUMERIC){
  	string compareLabel( NextLabel());
  	string trueLabel( NextLabel());

 	emitComment("while");
  	emit("j", compareLabel);
  	emitLabel(trueLabel);
  	myStmt -> emitCode();
  	emitLabel(compareLabel);
  	myExpression->emitCode();
  	emit("bnez", myExpression->Reg, trueLabel);
	freeReg(myExpression->Reg);
  }
  else{
  	string compareLabel( NextLabel());
  	string trueLabel( NextLabel());
	string falseLabel( NextLabel() );

  	emitComment("while");
	emit("j", compareLabel);
	emitLabel(trueLabel);
	myStmt -> emitCode();
  	emitLabel(compareLabel);
	evaluateCondition(myExpression, trueLabel, falseLabel);
	emitLabel(falseLabel);
  }
}

void ProcCallNode :: emitCode()
{
   Symbol * s = myIdentifier->getSymbol();
   emitComment("call");
   saveRegs();
   //throw args on stack
   myArgList->emitCode();
   emit("jal", s->symbolName+".f");
   restoreRegs();
}

void ReturnNode :: emitCode() {
  emitEmptyLine();
  emitComment("return");

  if( !myReturnVal->isNull()) {
	myReturnVal -> emitCode();
	emit( "move", "$v0", "$t"+IntToStr(myReturnVal->Reg) );
	freeReg( myReturnVal->Reg );
  }

  if(currentFunctionName=="main")
	emit( "j", currentFunctionName+".." );
  else
	emit( "j", currentFunctionName+".f.." );
}


// ===========================================================================



void OrNode :: emitCode()
{
   if(NUMERIC){	
	string falseLabel( NextLabel() );
	string endLabel( NextLabel() );

	myLeft->emitCode();
	string regL = "$t"+IntToStr(myLeft->Reg);
	emit("beqz", regL, falseLabel );
	emit("j", endLabel);
	emitLabel(falseLabel);

	freeReg(myLeft->Reg);
	myRight->emitCode();
	string regR = "$t"+IntToStr(myRight->Reg);
	emitLabel(endLabel);
	Reg = myRight->Reg;
   }
   else{
	// THIS IS CONTROL FLOW CODE ---------------------
	string falseLabel = NextLabel();
	string trueLabel = NextLabel();
	string endLabel = NextLabel();
	evaluateCondition(this, trueLabel, falseLabel);
	emitLabel(trueLabel);
	Reg = getReg();
  	emit( "li", Reg, 1 );
	emit( "j", endLabel);
	emitLabel(falseLabel);
	emit( "li", Reg, 0 ); 
	emitLabel(endLabel);
   }
}


void AndNode :: emitCode()
{
   if(NUMERIC){
	string endLabel( NextLabel() );
	myLeft->emitCode();
	string regL = "$t"+IntToStr(myLeft->Reg);
	emit("beqz", regL, endLabel);
	freeReg(myLeft->Reg); 
	myRight->emitCode();
	Reg = myRight->Reg;
	emitLabel(endLabel);
   }
   else{
	// THIS IS CONTROL FLOW CODE ---------------------
	string falseLabel = NextLabel();
	string trueLabel = NextLabel();
	string endLabel = NextLabel();
	evaluateCondition(this, trueLabel, falseLabel);
	emitLabel(trueLabel);
	Reg = getReg();
  	emit( "li", Reg, 1 );
	emit( "j", endLabel);
	emitLabel(falseLabel);
	emit( "li", Reg, 0 ); 
	emitLabel(endLabel);
   }
}


void RelOpNode :: emitCode()
{
   if(NUMERIC){  
	// THIS IS NUMERIC CODE -----------
	string op = setRelOp(myRelOp);
	int regL, regR;
	myLeft->emitCode();
	myRight->emitCode();
	regL = myLeft->  Reg;
	regR = myRight-> Reg;
	
	emit(op, "$t"+IntToStr(regL), "$t"+IntToStr(regL), "$t"+IntToStr(regR));
	Reg = regL;
	freeReg(regR); 
	// -------------------------------
   }
   else{
	// THIS IS CONTROL FLOW CODE---------------------
	string trueLabel = NextLabel();
	string falseLabel = NextLabel();
	string endLabel = NextLabel();
	evaluateCondition(this, trueLabel, falseLabel);
	
	// Might need to load a 1 or a 0 into Reg for this to work	
	emitLabel(trueLabel);
	Reg = getReg();
  	emit( "li", Reg, 1 );
	emit( "j", endLabel);
	emitLabel(falseLabel);
	emit( "li", Reg, 0 ); 
	emitLabel(endLabel);
	
   } 
}


void ArithOpNode :: emitCode()
{
	// ***************** ArithOpNode is finished ******************
  if(SethiUllmanFlag){
	doSethiUllmanLabeling();
	doSethiUllmanCodeGen();
  }
  else{
	string op = binaryOp(myArithOp);
	int regL, regR;
	myLeft->emitCode();
	myRight->emitCode();
	regL = myLeft->Reg;
	regR = myRight->Reg;
	
	emit(op, "$t"+IntToStr(regL), "$t"+IntToStr(regL), "$t"+IntToStr(regR));
	
	Reg = regL;
	freeReg(regR);
  }
}

// ===========================================================================


void FnCallNode :: emitCode()
{
   emitComment("call");
   saveRegs();
   //save args if any
   myArgList->emitCode();
   emit("jal", myIdentifier->getMyStringVal()+".f");
   Reg=getReg();
   emitComment("function value");
   emit("move", Reg, "$v0");
}

void NameVarNode :: emitCode()
{
   Reg = getReg();
   if(myIdentifier->getSymbol()->adr == Global && 
      myIdentifier->getSymbol()->symbolKind == VarKind){
   	string name = myIdentifier->getMyStringVal();
   	emit( "lw", Reg, name+".v" );   // SUFFIX(variable)
   }
   else if(myIdentifier->getSymbol()->symbolKind == ArrayKind &&
	   myIdentifier->getSymbol()->adr == Global){
   	string name = myIdentifier->getMyStringVal();
	emit("la", Reg, name+".v" );
   }
   else if(myIdentifier->getSymbol()->symbolKind == ArrayKind ||
		myIdentifier->getSymbol()->symbolKind == ArrayParamKind ){
	Symbol * s = myIdentifier->getSymbol();
	int o = s->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	emit( "addi", "$t"+IntToStr(Reg), "$fp", off);
   }
   else if(myIdentifier->getSymbol()->symbolKind == VarKind ||
	   myIdentifier->getSymbol()->symbolKind == ValParamKind){
	Symbol * s = myIdentifier->getSymbol();
	int o = s->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	string regStr = off+ "($fp)";
	emit( "lw", "$t"+IntToStr(Reg), regStr );
   }
   else{
	assert(0);
   }
}

void ArrayVarNode :: emitCode()
{
   //need for all 3
   myExpression->emitCode();
   int RegFromExp=myExpression->Reg; 
   emit("sll", "$t"+IntToStr(RegFromExp), "$t"+IntToStr(RegFromExp), "2");

   //global only
   if(myIdentifier->getSymbol()->adr == Global){
	Reg=getReg();
	string str(myIdentifier->getMyStringVal());
	emit("la","$t"+IntToStr(Reg), str+".v");
	emit("add","$t"+IntToStr(RegFromExp), "$t"+IntToStr(RegFromExp)
		  , "$t"+IntToStr(Reg));
	emit("lw", "$t"+IntToStr(RegFromExp), "($t"+IntToStr(RegFromExp)+")");
	freeReg(Reg); //am i really done with it?
	Reg = RegFromExp;
   }
   else if(myIdentifier->getSymbol()->adr == Local 
        && myIdentifier->getSymbol()->symbolKind == ArrayKind){
   //local
	Symbol * s = myIdentifier->getSymbol();
	emit("add", "$t"+IntToStr(RegFromExp), "$fp", "$t"+IntToStr(RegFromExp));
	int o = s->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	emit("lw", "$t"+IntToStr(RegFromExp),
		 off+"($t"+IntToStr(RegFromExp)+")");
	//freeReg(Reg);
	Reg = RegFromExp;
   }
   else{
   //Param
	Reg=getReg();
	Symbol * s = myIdentifier->getSymbol();
	int o = s->offset;
	string off = IntToStr(o);
	if(o > 0) off= "-"+off;
	string regStr = off+ "($fp)";
	emit("lw", "$t"+IntToStr(Reg), regStr);
	emit("add", "$t"+IntToStr(RegFromExp), "$t"+IntToStr(Reg), 
		  "$t"+IntToStr(RegFromExp));
	emit("lw", "$t"+IntToStr(RegFromExp), "0($t"+IntToStr(RegFromExp)+")");
	freeReg(Reg); //ami really done with it though?
	Reg = RegFromExp;
   }
}

void IntLiteralNode :: emitCode()
{
  Reg = getReg();
  emit( "li", Reg, myIntVal );
}

void TrueNode :: emitCode()
{
  Reg = getReg();
  emit( "li", Reg, 1 );
}

void FalseNode :: emitCode()
{
  Reg = getReg();
  emit( "li", Reg, 0 );
}

// ===========================================================================

void TargetNode :: emitCode()
{
   // Explicitly tell the target it is the fp
   // since all local vars are offset from fp
   // and globals don't use registers
   Reg = 30;
   Symbol *ptrL = getID()->getSymbol();

   if(ptrL->adr == Global && ptrL->symbolKind == VarKind){
	return;
   }
   else if( ptrL->adr == Global && ptrL->symbolKind == ArrayKind){
	getExp()->emitCode();
	int regEx = getExp()->Reg;
	int global = getReg();
	string regStr = "$t"+IntToStr(regEx);
	emit("sll", regStr, regStr, IntToStr(2));
	emit("la", "$t"+IntToStr(global), ptrL->symbolName+".v");
	emit("add", regStr, regStr, "$t"+IntToStr(global));
	freeReg(global); 
	Reg = regEx;
   }
   else if( ptrL->adr == Local && 
	  ( ptrL->symbolKind == VarKind || ptrL->symbolKind == ValParamKind )){
	return;
   }
   else if( ptrL->adr == Local && ptrL->symbolKind == ArrayKind){
	getExp()->emitCode();
	int regEx = getExp()->Reg;
	string regStr = "$t"+IntToStr(regEx);
	emit("sll", regStr, regStr, IntToStr(2));
	emit("add", regStr, "$fp", regStr); 
	Reg = regEx;
   }
   else if( ptrL->adr == Local && ptrL->symbolKind == ArrayParamKind){
	getExp()->emitCode();
	int reg = getReg();
	int regEx = getExp()->Reg;
	int o = ptrL->offset;
	string regS = "$t"+IntToStr(reg);
	string regStr = "$t"+IntToStr(regEx);
	string off = IntToStr(o)+"($fp)";
	if(o != 0) off = "-" + off;
	emit("sll", regStr, regStr, IntToStr(2));
	emit("lw", regS, off);
	emit("add", regStr, regS, regStr);
	Reg = regEx;
	freeReg(reg);
   }
}

void StringLiteralNode :: emitCode()
{
  string label;

  if ( *myStringVal == "\"\\n\"" )
	label = ".nl";
  else{
	if(l.find((*myStringVal)) == l.end()){
		label = NextLabel();
		emitStringDefinition( label, *myStringVal );
		l[(*myStringVal)] = label;
	}
	else{
		label = l[*myStringVal];
	}
  }

  // load into $a0 so it can be printed; can do it here
  // because a string literal is only used in print statements
  emit( "la", "$a0", label );
}












// ===========================================================================
//
//          H I G H E R - L E V E L     E M I T     R O U T I N E S
//
// ===========================================================================


void emitPrintString()
{
  emit( "li", "$v0", 4 );
  emit( "syscall", "\t\t# print_string" );
}
 
void emitPrintInt( int reg )
{
  emit( "move", "$a0", "$t"+IntToStr(reg) );
  emit( "li"  , "$v0", 1 );
  emit( "syscall", "\t\t# print_int" );
}

void emitPrintBool( int reg )
{
  string falseLabel = NextLabel();
  string endLabel = NextLabel();
  emit( "beqz", "$t"+IntToStr(reg), falseLabel); 
  emit( "la", "$a0", ".true" );
  emit( "j", endLabel);
  emitLabel(falseLabel);
  emit( "la", "$a0", ".false" );
  emitLabel(endLabel);
  emit( "li", "$v0", 4);
  emit( "syscall", "\t\t# print_bool" );
}


// ===========================================================================

void emitL
 (string label, string opcode,string arg1,string arg2,string arg3)
{
  asmFile << label << ":\t" ;
  if ( opcode=="" ) {asmFile<<endl; return;} else asmFile << opcode;
  if ( arg1  =="" ) {asmFile<<endl; return;} else asmFile << arg1;
  if ( arg2  =="" ) {asmFile<<endl; return;} else asmFile << arg2;
  if ( arg3  =="" ) {asmFile<<endl; return;} else asmFile << arg3;
  asmFile<<endl;
  asmFile.flush();
}


// ===========================================================================

void emitProgramHeader()
{
  emit ( ".data" );
  emitL( ".true" , ".asciiz\t", "\"true\""  );
  emitL( ".false", ".asciiz\t", "\"false\"" );
  emitL( ".nl"   , ".asciiz\t", "\"\\n\"" );
  emit ( ".text" );
  emit ( ".globl  main" );
  emitEmptyLine();
}


void emitProgramFooter()
{
  asmFile.close();
}




// ===========================================================================




void emitFunctionPrologue( string fn, int frameSize, int localVarCount )
{
  emitEmptyLine();
  if(fn!="main") emitLabel( fn+".f" ); else emitLabel( fn );
  emitBlockComment( "PROLOGUE" );

  string space( IntToStr(localVarCount*4+8) );
  if (localVarCount == 0)
       emit( "subu", "$sp", "$sp", "8"  , "# registers ra & fp" );
  else
       emit( "subu", "$sp", "$sp", space, "# ra, fp & locals"   );
  emit( "sw"  , "$ra", IntToStr(localVarCount*4+4)+"($sp)" );
  emit( "sw"  , "$fp", IntToStr(localVarCount*4  )+"($sp)" );

  emit( "addu", "$fp", "$sp", IntToStr(frameSize-4), "# set new fp" );
}




void emitFunctionEpilogue( string fn, int frameSize, int parameterCount )
{
  emitEmptyLine();
  if(fn!="main") emitLabel( fn+".f.." ); else emitLabel( fn+".." );
  emitBlockComment( "EPILOGUE" );

  emitOffset( "lw"  , "$ra", parameterCount*4,   "$fp", "# restore ra" );
  emitOffset( "lw"  , "$fp", parameterCount*4+4, "$fp", "# restore fp" );

  emit( "addu", "$sp", "$sp", IntToStr(frameSize), "# Pop" );
}

// ===========================================================================

void declareGlobalVar( string name )
{
  emit ( ".data" );
  emit ( ".align 2" );
  emitL( name, ".space 4" );  //always 4 for simplicity
  emit ( ".text" );
  emitEmptyLine();
}


void declareGlobalArray( string name, int size )
{
  emit ( ".data" );
  emit ( ".align 2" );
  emitL( name, ".space "+IntToStr(size) );
  emit ( ".text" );
  emitEmptyLine();
}


void emitStringDefinition( string label, string String )
{
  emitEmptyLine();
  emit ( ".data" );
  emitL( label, ".asciiz\t ", String );
  emit ( ".text" );
  emitEmptyLine();
}



// ===========================================================================
//
//                      S E T H I  -  U L L M A N
//
// ===========================================================================

void ArithOpNode    :: doSethiUllmanLabeling() {
  myLeft->doSethiUllmanLabeling();
  myRight->doSethiUllmanLabeling();
  if(myLeft->lbl == myRight->lbl){
	  lbl = myLeft->lbl+1;
  } 
  else if(myLeft->lbl > myRight->lbl){
	lbl = myLeft->lbl;
  }
  else{
	lbl = myRight->lbl;
  }
}

void IntLiteralNode :: doSethiUllmanLabeling() {
  lbl = 1;
}
void NameVarNode    :: doSethiUllmanLabeling() {
  lbl = 1;
}
void ArrayVarNode   :: doSethiUllmanLabeling() {
  lbl = 1;
}
void FnCallNode     :: doSethiUllmanLabeling() {
  lbl = 1;
}

void ArithOpNode    :: doSethiUllmanCodeGen()  {
	string op = binaryOp(myArithOp);
	int regL, regR;

	if(myLeft->lbl > myRight->lbl){
		myLeft->doSethiUllmanCodeGen();
		myRight->doSethiUllmanCodeGen();
	}
	if(myRight->lbl >= myLeft->lbl){
		myRight->doSethiUllmanCodeGen();
		myLeft->doSethiUllmanCodeGen();
	}

	regL = myLeft->Reg;
	regR = myRight->Reg;
	
	emit(op, "$t"+IntToStr(regL), "$t"+IntToStr(regL), "$t"+IntToStr(regR));
	
	Reg = regL;
	freeReg(regR);
}

void IntLiteralNode :: doSethiUllmanCodeGen()  { emitCode(); }
void NameVarNode    :: doSethiUllmanCodeGen()  { emitCode(); }
void ArrayVarNode   :: doSethiUllmanCodeGen()  { emitCode(); }
void FnCallNode     :: doSethiUllmanCodeGen()  { emitCode(); }
