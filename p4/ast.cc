// ast.cc

#include "symbol.h"
#include "ast.h"

ProgramNode :: ProgramNode(GlobalListNode*g, FunctionListNode*f, int l, int c)
            :  ASTnode(l,c),  myGlobalList(g), myFunctionList(f)
            {}

GlobalListNode :: GlobalListNode(GlobalListNode*g, DeclNode*v, int l, int c)
               :  ASTnode(l,c),  myGlobalList(g), myDecl(v)
               {}

FunctionListNode
       :: FunctionListNode(FunctionListNode*f, FunctionDeclNode*d, int l, int c)
       :  ASTnode(l,c),  myFunctionList(f), myFunctionDecl(d)
       {}

FunctionDeclNode
        :: FunctionDeclNode(TypeNode*t, IdentifierNode*i, ParametersNode*p,
                            VarListNode*v, StmtListNode*s, int l, int c)
        :  ASTnode(l,c), myType(t), myIdentifier(i), myParameters(p),
                         myVarList(v), myStmtList(s)
        {}

TypeNode :: TypeNode(int l, int c) : ASTnode(l,c) {}

TypeIntNode  :: TypeIntNode (int l, int c) : TypeNode(l,c) {}
TypeBoolNode :: TypeBoolNode(int l, int c) : TypeNode(l,c) {}


DeclNode :: DeclNode(int l, int c) : ASTnode(l,c) {}

VariableDeclNode :: VariableDeclNode(TypeNode *t, IdentifierNode *i, int l, int c)
                 :  DeclNode(l,c), myType(t), myIdentifier(i)
                 {}

ArrayDeclNode
     :: ArrayDeclNode(TypeNode*t, IdentifierNode*i, ExpressionNode*n, int l,int c)
     :  DeclNode(l,c), myType(t), myIdentifier(i), myIntLit(n)
     {}

ParametersNode :: ParametersNode(ParamListNode* p, int l, int c)
               :  ASTnode(l,c), myParamList(p)
               {}

ParamListNode :: ParamListNode(ParamListNode* p, ParamNode* n, int l, int c)
              :  ASTnode(l,c), myParamList(p), myParam(n)
              {}

ArgListNode :: ArgListNode(ArgListNode *a, ExpressionNode *e, int l, int c)
            :  ASTnode(l,c),  myArgList(a),   myExpression(e)
            {}

ParamNode :: ParamNode(int l, int c) :  ASTnode(l,c) {}

ParamValNode :: ParamValNode(TypeNode* t, IdentifierNode* i, int l, int c)
             :  ParamNode(l,c), myType(t),   myIdentifier(i)
             {}

ParamArrayNode :: ParamArrayNode(TypeNode* t, IdentifierNode* i, int l,int c)
               :  ParamNode(l,c),   myType(t),   myIdentifier(i)
               {}

VarListNode :: VarListNode(VarListNode *v, DeclNode *d, int l, int c)
            :  ASTnode(l,c),  myVarList(v),   myDecl(d)
            {}

StmtListNode :: StmtListNode(StmtListNode *s, StmtNode *n, int l, int c)
             :  ASTnode(l,c),   myStmtList(s),   myStmt(n)
             {}


StmtNode :: StmtNode(int l, int c) : ASTnode(l,c) {}

AssignNode :: AssignNode(TargetNode  *t, ExpressionNode *e, int l, int c)
           :  StmtNode(l,c), myTarget(t),   myExpression(e)
           {}

IfNode :: IfNode(ExpressionNode *ex, StmtNode *t, StmtNode *e, int l, int c)
       :  StmtNode(l,c), myExpression(ex), myThenStmt(t), myElseStmt(e)
       {}

WhileNode :: WhileNode(ExpressionNode *e, StmtNode *s, int l, int c)
          :  StmtNode(l,c), myExpression(e), myStmt(s)
          {}

BlockNode :: BlockNode( VarListNode  *v, StmtListNode *s, int l, int c)
          :  StmtNode(l,c), myVarList(v),   myStmtList(s)
          {}

ReturnNode :: ReturnNode(ExpressionNode *e, int l,int c)
           :  StmtNode(l,c), myReturnVal(e)
           {}

ProcCallNode :: ProcCallNode(IdentifierNode*i, ArgListNode*a, int l, int c)
             :  StmtNode(l,c), myIdentifier(i),  myArgList(a)
             {}

PrintNode :: PrintNode(PrintListNode *p, int l, int c)
          :  StmtNode(l,c), myPrintList(p)
          {}

CoutNode :: CoutNode(CoutListNode *p, int l, int c)
	 :  StmtNode(l,c), myCoutList(p)
	 {}

PrintListNode :: PrintListNode(PrintListNode *p, PrintItemNode *i, int l, int c)
              :  ASTnode(l,c), myPrintList(p), myPrintItem(i)
              {}

CoutListNode :: CoutListNode(CoutListNode *p, PrintItemNode *i, int l, int c)
	     :  ASTnode(l,c), myCoutList(p), myCoutItem(i)
	     {}

PrintItemNode :: PrintItemNode(int l, int c) : ASTnode(l,c) {}

ExpressionNode :: ExpressionNode(int l, int c) : PrintItemNode(l,c) {}

OrNode :: OrNode(ExpressionNode *e1, ExpressionNode *e2, int l,int c)
           :  ExpressionNode(l,c), myLeft(e1), myRight(e2)
           {}

AndNode :: AndNode(ExpressionNode *e1, ExpressionNode *e2, int l,int c)
           :  ExpressionNode(l,c), myLeft(e1), myRight(e2)
           {}

RelOpNode :: RelOpNode(int o, ExpressionNode *e1, ExpressionNode *e2, int l,int c)
          :  ExpressionNode(l,c), myRelOp(o), myLeft(e1), myRight(e2)
          {}

ArithOpNode :: ArithOpNode
                    (int o,ExpressionNode *e1,ExpressionNode *e2, int l,int c)
            :  ExpressionNode(l,c), myArithOp(o), myLeft(e1), myRight(e2)
            {}

TrueNode  :: TrueNode (int l, int c) : ExpressionNode(l,c) {}
FalseNode :: FalseNode(int l, int c) : ExpressionNode(l,c) {}

NameVarNode :: NameVarNode( IdentifierNode *n, int l, int c)
            :  ExpressionNode(l,c),  myIdentifier(n)
            {}

ArrayVarNode :: ArrayVarNode( IdentifierNode *n, ExpressionNode *e, int l, int c)
             :  ExpressionNode(l,c),  myIdentifier(n), myExpression(e)
             {}

FnCallNode :: FnCallNode(IdentifierNode *i, ArgListNode *a, int l, int c)
           :  ExpressionNode(l,c),  myIdentifier(i), myArgList(a)
           {}

IntLiteralNode :: IntLiteralNode(int n, int l, int c)
               :  ExpressionNode(l,c)
               { myIntVal=n; }


TargetNode :: TargetNode(IdentifierNode *i, ExpressionNode *e, int l, int c)
           :  ASTnode(l,c), myIdentifier(i), myExpression(e)
           {}

IdentifierNode :: IdentifierNode(string *s, int l, int c)
               :  ASTnode(l,c),  myStringVal(s)
               {}

StringLiteralNode :: StringLiteralNode(string *s, int l, int c)
                  :  PrintItemNode(l,c),  myStringVal(s)
                  {}
