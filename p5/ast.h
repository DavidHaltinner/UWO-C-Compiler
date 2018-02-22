// ast.h

#ifndef AST_H_
#define AST_H_

#include <iostream>
#include <iomanip>
#include "symbol.h"

// forward
class  GlobalListNode	;
class  FunctionListNode	;
class  DeclNode		;
class  FunctionDeclNode	;
class  TypeNode		;
class  ParametersNode	;
class  ParamListNode	;
class  ParamNode	;
class  VarListNode	;
class  StmtListNode	;
class  StmtNode		;
class  ArgListNode	;
class  PrintListNode	;
class  PrintItemNode	;
class  ExpressionNode	;
class  TargetNode	;
class  IntLiteralNode	;
class  CoutListNode	;
class  CoutItemNode     ;


class ASTnode   // ABSTRACT BASE CLASS
{
 public:

	// every node will have a line number and a column number
	// so unparsing can produce a kind of pretty-printing
	const int line  ;
	const int column;
	void doLineNum() { cout << setw(6) << line << ": "; }
	int Reg;
	int lbl;

	SymbolType nodeType;

	ASTnode(int l, int c) : line(l), column(c) {};

	virtual bool isNull ()  { return false; }
	virtual bool isBlock()  { return false; }
	virtual bool isName()	{ return false; }
	virtual bool isOr()     { return false; }
	virtual bool isAnd()    { return false; }
	virtual bool isRel()    { return false; }
	virtual bool isTrue()   { return false; }
	virtual bool isFalse()  { return false; }
		
	virtual void unparse(int indent=0) =0;
	
	virtual void checkNames() =0;
	virtual void checkTypes() =0;
	virtual void emitCode() =0;
 private:

	// disable copy constructor and copy assignment operator
	ASTnode(const ASTnode & n);
	ASTnode & operator= (const ASTnode & n);
};


class IdentifierNode : public ASTnode 
{
 private:
	string * myStringVal;
	Symbol * mySymbolTableEntry;
 public:
	IdentifierNode(string *, int, int);
	string getMyStringVal() { return * myStringVal; }
	void setMySymbolTableEntry(Symbol * s) { mySymbolTableEntry = s; }
	Symbol * getSymbol() { return mySymbolTableEntry; }
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode() {return;}
};


class ProgramNode : public ASTnode {
 private:
	GlobalListNode   * myGlobalList;
	FunctionListNode * myFunctionList;
 public:
	ProgramNode(GlobalListNode*, FunctionListNode*, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class GlobalListNode : public ASTnode {
 private:
	GlobalListNode * myGlobalList;
	DeclNode       * myDecl;
 public:
	GlobalListNode(GlobalListNode*, DeclNode*, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class FunctionListNode : public ASTnode
{
 private:
	FunctionListNode * myFunctionList;
	FunctionDeclNode * myFunctionDecl;
 public:
	FunctionListNode(FunctionListNode*, FunctionDeclNode*, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class FunctionDeclNode : public ASTnode
{
 private:
	TypeNode       * myType;
	IdentifierNode * myIdentifier;
	ParametersNode * myParameters;
	VarListNode    * myVarList;
	StmtListNode   * myStmtList;
	Symbol * mySymbolTableEntry;
 public:
	FunctionDeclNode( TypeNode*, IdentifierNode*, ParametersNode*,
                          VarListNode*, StmtListNode*, int, int);
	void setMySymbolTableEntry(Symbol * s) { mySymbolTableEntry = s; }
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class TypeNode : public ASTnode  // ABSTRACT BASE CLASS
{
 public:
	TypeNode(int, int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() = 0;
	virtual void checkTypes() = 0;
};
	// --------------- SUBCLASSES OF TypeNode ---------------
	class TypeIntNode : public TypeNode
	{
	 public:
		TypeIntNode(int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class TypeBoolNode : public TypeNode
	{
	 public:
		TypeBoolNode(int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};
	// --------------- SUBCLASSES OF TypeNode ---------------

class DeclNode : public ASTnode  // ABSTRACT BASE CLASS
{
 public:
	DeclNode(int, int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() = 0;
	virtual void checkTypes() = 0;
	virtual void emitCode()   = 0;
};
	// --------------- SUBCLASSES OF DeclNode ---------------

	class VariableDeclNode : public DeclNode
	{
	 private:
		TypeNode       * myType;
		IdentifierNode * myIdentifier;
	 public:
		VariableDeclNode(TypeNode*, IdentifierNode*, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class ArrayDeclNode : public DeclNode
	{
	 private:
		TypeNode       * myType;
		IdentifierNode * myIdentifier;
		ExpressionNode * myIntLit;
	 public:
		ArrayDeclNode(TypeNode*, IdentifierNode*, ExpressionNode*, int,int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};
	// --------------- SUBCLASSES OF DeclNode ---------------


class ParametersNode : public ASTnode
{
 private:
	ParamListNode * myParamList;
 public:
	ParametersNode :: ParametersNode(ParamListNode*, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class ParamListNode : public ASTnode
{
 private:
	ParamListNode * myParamList;
	ParamNode     * myParam    ;
 public:
	ParamListNode :: ParamListNode(ParamListNode*, ParamNode*, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class ParamNode : public ASTnode   // ABSTRACT BASE CLASS
{
 public:
	ParamNode :: ParamNode(int, int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() =0;
	virtual void checkTypes() =0;
	virtual void emitCode()   =0;
};
	// --------------- SUBCLASSES OF ParamNode ---------------
	class ParamValNode : public ParamNode
	{
	 private:
		TypeNode       * myType      ;
		IdentifierNode * myIdentifier;
	 public:
		ParamValNode :: ParamValNode(TypeNode*, IdentifierNode*, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class ParamArrayNode : public ParamNode
	{
	 private:
		TypeNode       * myType      ;
		IdentifierNode * myIdentifier;
	 public:
		ParamArrayNode::ParamArrayNode(TypeNode*, IdentifierNode*, int,int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};
	// --------------- SUBCLASSES OF ParamNode ---------------


class VarListNode : public ASTnode
{
 private:
	VarListNode * myVarList;
	DeclNode    * myDecl;
 public:
	VarListNode(VarListNode *, DeclNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();            
	virtual void emitCode();
};


class StmtListNode : public ASTnode
{
 private:
	StmtListNode * myStmtList;
	StmtNode     * myStmt;
 public:
	StmtListNode(StmtListNode *, StmtNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class StmtNode : public ASTnode   // ABSTRACT BASE CLASS
{
 public:
	StmtNode(int, int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() =0;
	virtual void checkTypes() =0;
	virtual void emitCode()   =0;
};
	// --------------- SUBCLASSES OF StmtNode ---------------
	class AssignNode : public StmtNode
	{
	 private:
		TargetNode     * myTarget;
		ExpressionNode * myExpression;
	 public:
		AssignNode(TargetNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class IfNode : public StmtNode
	{
	 private:
		ExpressionNode * myExpression;
		StmtNode       * myThenStmt;
		StmtNode       * myElseStmt;
	 public:
		IfNode(ExpressionNode *, StmtNode *, StmtNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class WhileNode : public StmtNode
	{
	 private:
		ExpressionNode * myExpression;
		StmtNode       * myStmt;
	 public:
		WhileNode(ExpressionNode *, StmtNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class BlockNode : public StmtNode
	{
	 private:
	        VarListNode  * myVarList;
	        StmtListNode * myStmtList;
	 public:
	        BlockNode( VarListNode*, StmtListNode*, int, int);
		virtual bool isBlock() { return true; }
	        virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class ReturnNode : public StmtNode
	{
	 private:
		ExpressionNode * myReturnVal;
	 public:
		ReturnNode(ExpressionNode*, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class ProcCallNode : public StmtNode
	{
	 private:
		IdentifierNode * myIdentifier;
		ArgListNode    * myArgList   ;
	 public:
		ProcCallNode(IdentifierNode*, ArgListNode*, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class PrintNode : public StmtNode
	{
	 private:
		PrintListNode * myPrintList;
	 public:
		PrintNode(PrintListNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
	};

	class CoutNode : public StmtNode
	{
	 private:
		CoutListNode * myCoutList;
	 public:
		CoutNode(CoutListNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames() {return;}
		virtual void checkTypes() {return;}
		virtual void emitCode()   {return;}
	};
	// --------------- SUBCLASSES OF StmtNode ---------------



class ArgListNode : public ASTnode
{
 private:
	ArgListNode    * myArgList   ;
	ExpressionNode * myExpression;

 public:
	ArgListNode(ArgListNode *, ExpressionNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class PrintListNode : public ASTnode
{
 private:
	PrintListNode * myPrintList;
	PrintItemNode * myPrintItem;
 public:
	PrintListNode(PrintListNode *, PrintItemNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};

class CoutListNode : public ASTnode
{
 private:
	CoutListNode * myCoutList;
	PrintItemNode * myCoutItem;
 public:
	CoutListNode(CoutListNode *, PrintItemNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames() {return;}
	virtual void checkTypes() {return;}
	virtual void emitCode() {return;}
};

class PrintItemNode : public ASTnode   // ABSTRACT BASE CLASS
{
 public:
	SymbolType myType;
	Symbol * mySymbolTableEntry;
	void setMySymbolTableEntry(Symbol * s) { mySymbolTableEntry = s; }
	PrintItemNode(int,int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() =0;
	virtual void checkTypes() =0;
	virtual void emitCode()   =0;
};

class ExpressionNode : public PrintItemNode   // ABSTRACT BASE CLASS
{
 public:
	int myIntVal;

	ExpressionNode(int,int);
	virtual void unparse(int indent=0) =0;
	virtual void checkNames() =0;
	virtual void checkTypes() =0;
	virtual void emitCode()   =0;
	int emitRight() { return -1;}
	int emitLeft() {  return -1;}
	ExpressionNode * getL() {return NULL;}
	ExpressionNode * getR() {return NULL;}
	virtual IdentifierNode * getID() {return NULL;}
	virtual void doSethiUllmanLabeling() =0;
	virtual void doSethiUllmanCodeGen()  =0;
	int getOp() {return -1;}
};
	// --------------- SUBCLASSES OF ExpressionNode ---------------
	class OrNode : public ExpressionNode
	{
	 private:
		ExpressionNode * myLeft  ;
		ExpressionNode * myRight ;
	 public:
		OrNode(ExpressionNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling() { return; }
		virtual void doSethiUllmanCodeGen()  { return; }
		virtual bool isOr()     { return true; }
		ExpressionNode * getL() {return myLeft;}
		ExpressionNode * getR() {return myRight;}
	};

	class AndNode : public ExpressionNode
	{
	 private:
		ExpressionNode * myLeft  ;
		ExpressionNode * myRight ;
	 public:
		AndNode(ExpressionNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling() { return; }
		virtual void doSethiUllmanCodeGen()  { return; }
		virtual bool isAnd()     { return true; }
		ExpressionNode * getL() {return myLeft;}
		ExpressionNode * getR() {return myRight;}
	};

	class RelOpNode : public ExpressionNode
	{
	 private:
		int              myRelOp;
		ExpressionNode * myLeft ;
		ExpressionNode * myRight;
	 public:
		RelOpNode(int, ExpressionNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling() { return; }
		virtual void doSethiUllmanCodeGen()  { return; }
		int emitRight() { myRight->emitCode(); return myRight->Reg;}
		int emitLeft() { myLeft->emitCode(); return myLeft->Reg;}
		int getOp() { return myRelOp;}
		virtual bool isRel()     { return true; }
	};

	class ArithOpNode : public ExpressionNode
	{
	 private:
		int              myArithOp;
		ExpressionNode * myLeft ;
		ExpressionNode * myRight;
	 public:
		ArithOpNode(int, ExpressionNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling();
		virtual void doSethiUllmanCodeGen();
	};

	class FnCallNode : public ExpressionNode
	{
	 private:
		IdentifierNode * myIdentifier;
		ArgListNode    * myArgList   ;
	 public:
		FnCallNode(IdentifierNode *, ArgListNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling();
		virtual void doSethiUllmanCodeGen();
		IdentifierNode * getID() { return myIdentifier; }
	};

	class TrueNode : public ExpressionNode
	{
	 public:
		TrueNode(int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling() { return; }
		virtual void doSethiUllmanCodeGen()  { return; }
		virtual bool isTrue()     { return true; }
	};

	class FalseNode : public ExpressionNode
	{
	 public:
		FalseNode(int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling() { return; }
		virtual void doSethiUllmanCodeGen()  { return; }
		virtual bool isFalse()     { return true; }
	};

	class NameVarNode : public ExpressionNode
	{
	 private:
		IdentifierNode * myIdentifier;
	 public:
		NameVarNode( IdentifierNode *, int, int);
		virtual bool isName() { return true;}
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling();
		virtual void doSethiUllmanCodeGen();
		virtual IdentifierNode * getID() {return myIdentifier;}
	};

	class ArrayVarNode : public ExpressionNode
	{
	 private:
		IdentifierNode * myIdentifier;
		ExpressionNode * myExpression;
	 public:
		ArrayVarNode( IdentifierNode *, ExpressionNode *, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling();
		virtual void doSethiUllmanCodeGen();
	};

	class IntLiteralNode : public ExpressionNode
	{
	 public:
		IntLiteralNode(int, int, int);
		virtual void unparse(int indent=0);
		virtual void checkNames();
		virtual void checkTypes();
		virtual void emitCode();
		virtual void doSethiUllmanLabeling();
		virtual void doSethiUllmanCodeGen();
	};
	// --------------- SUBCLASSES OF ExpressionNode ---------------


class TargetNode : public ASTnode 
{
 private:
	IdentifierNode * myIdentifier;
	ExpressionNode * myExpression;
 public:
	TargetNode(IdentifierNode *, ExpressionNode *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
	IdentifierNode * getID() { return myIdentifier; }
	ExpressionNode * getExp() {return myExpression; }
};


class StringLiteralNode : public PrintItemNode 
{
 private:
	string * myStringVal;
 public:
	StringLiteralNode(string *, int, int);
	virtual void unparse(int indent=0);
	virtual void checkNames();
	virtual void checkTypes();
	virtual void emitCode();
};


class NullNode : public ASTnode
{
 public:
	NullNode() : ASTnode(0,0) {}
	virtual bool isNull() { return true; }
	virtual void unparse(int indentVal=0) { return; }
	virtual void checkNames() { return; };
	virtual void checkTypes() { return; };
	virtual void emitCode() {return;}
};

extern ASTnode * astRoot;

#endif
