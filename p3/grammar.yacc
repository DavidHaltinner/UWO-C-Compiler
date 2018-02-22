/* grammar.yacc */

%{ // stuff here is copied directly into y.tab.c above (outside of) yyparse()

#include "ast.h"
#include "scanner.h"

// bison needs these two forward declarations (but yacc doesn't)
extern int yylex();
int yyerror( char *msg);

// just need "one" null node
// don't need to create a "new" null node every time we need one
static NullNode * NULLnode = new NullNode();

%}

/*
   define the data type of the value stack (YYSTYPE)
   the "value" associated with some tokens  will be data type TokenType
   the "value" associated with nonterminals will be data type ...Node *
*/

%union{
	TokenType  t;

	ProgramNode		* Program	;
	GlobalListNode		* GlobalList	;
	FunctionListNode	* FunctionList	;
	FunctionDeclNode	* FunctionDecl	;
	TypeNode		* Type		;
	ParametersNode		* Parameters	;
	ParamListNode		* ParamList	;
	ParamNode		* Param		;
	VarListNode		* VarList	;
	DeclNode		* Decl		;
	ArgListNode		* ArgList	;
	StmtListNode		* StmtList	;
	StmtNode		* Stmt		;
	PrintListNode		* PrintList	;
	PrintItemNode		* PrintItem	;
	CoutListNode		* CoutList      ;
	CoutItemNode		* CoutItem      ;
	OrNode			* Or		;
	AndNode			* And		;
	RelOpNode		* RelOp		;
	ArithOpNode		* ArithOp	;
	ExpressionNode		* Expression	;
	TargetNode		* Target	;
	IdentifierNode		* Identifier	;
	StringLiteralNode	* StringLiteral	;
      }

/*
   since the value stack is a struct(union), it seems we need to use the
   notation $2.t or $$.a;  the actual ugly notation is  $<t>2  or  $<a>$
   the %type directive lets us use $2 or $$  because it tells yacc/bison
   which member of the struct(union) should be referenced
*/

      /*** do not change these token and type specifications! ***/
/* ====================================================================== */
%token     SEMICOLON RPAREN RBRACE RBRACKET
%token <t> ID INTLITERAL STRINGLITERAL ASSIGN PRINT INT VOID TRUE FALSE
%token <t> LPAREN LBRACE LBRACKET PLUS MINUS TIMES SLASH BOOL AND OR
%token <t> IF ELSE WHILE RETURN LT GT LEQ GEQ EQ NEQ COMMA
%token <t> COUT AARROW

%type <Program>		program
%type <GlobalList>	globalList
%type <FunctionList>	functionList
%type <FunctionDecl>	functionDecl
%type <Type>		type
%type <Decl>		variableDecl
%type <Parameters>	parameters
%type <ParamList>	paramList
%type <Param>		param
%type <VarList>		varList
%type <StmtList>	stmtList
%type <ArgList>		argList
%type <Stmt>		stmt
%type <PrintList>	printList
%type <PrintItem>	printItem
%type <CoutList>	coutList
%type <PrintItem>       coutItem

%type <Expression>	expression
%type <Expression>	term
%type <Expression>	factor
%type <Expression>	primary
%type <Expression>	unit
%type <Expression>	nameVar
%type <Expression>	arrayVar
%type <Expression>	intLiteral

%type <Expression>      ORexpression
%type <Expression>      ANDexpression
%type <Expression>      ORlist
%type <Expression>      ANDlist

%type <Target>		target
%type <Identifier>	identifier
%type <StringLiteral>	stringLiteral
/* ====================================================================== */

%nonassoc THEN
%nonassoc ELSE

%start program

%%


program	: globalList  functionList
	      {
		astRoot = new ProgramNode($1,$2, $1->line,$1->column);
	      }
	;

globalList : globalList  variableDecl
		{ $$ = new GlobalListNode($1, $2, $1->line,$1->column ); }
	   |
		{ $$ = (GlobalListNode *)NULLnode; }
	   ;

functionList : functionList functionDecl
		{ $$ = new FunctionListNode($1, $2, $2->line, $2->column); } 
	     | functionDecl
		{$$ = new FunctionListNode(
		      (FunctionListNode *)NULLnode, $1,$1->line,$1->column);}
	     ;

type : INT  {$$ = new TypeIntNode ( $1.line,$1.column ); } 
     | BOOL {$$ = new TypeBoolNode( $1.line, $1.column); } 
     ;

variableDecl : type identifier SEMICOLON
		{$$ = new VariableDeclNode( $1,$2, $2->line,$2->column );}
	     | type identifier LBRACKET intLiteral RBRACKET SEMICOLON
		{$$ = new ArrayDeclNode($1, $2, $4, $2->line, $2->column );}
	     ;

functionDecl : VOID identifier parameters LBRACE varList stmtList RBRACE
	       {$$ = new FunctionDeclNode(
		      (TypeNode *)NULLnode, $2,$3,$5,$6,$2->line,$2->column);}
	     | type identifier parameters LBRACE varList stmtList RBRACE
	       {$$ = new FunctionDeclNode($1, $2, $3, $5, $6, $2->line, $2->column);}
	     ;

parameters : LPAREN RPAREN
		{ ParamListNode * n = (ParamListNode *)NULLnode;
                  $$ = new ParametersNode(  n, $1.line,$1.column ); }
	   | LPAREN paramList RPAREN
		{ $$ = new ParametersNode($2, $1.line, $1.column); }
	   ;

paramList : paramList COMMA param 
		{ $$ = new ParamListNode($1, $3, $1->line, $1->column); }
	  | param
		{ $$ = new ParamListNode( (ParamListNode *)NULLnode, $1, 
						$1->line, $1->column); }
	  ;

param : type identifier
		{ $$ = new ParamValNode($1, $2, $1->line, $1->column); }
      | type identifier LBRACKET RBRACKET
		{ $$ = new ParamArrayNode($1, $2, $1->line, $1->column); }
      ;
 
varList : { $$ = (VarListNode *)NULLnode; } 
	| varList variableDecl
		{ $$ = new VarListNode($1, $2, $1->line, $1->column); }
	;

stmtList : stmtList stmt
		{ $$ = new StmtListNode( $1, $2, $1->line,$1->column ); }
	 |
		{ $$ = (StmtListNode *)NULLnode; }
	 ;

stmt	: target ASSIGN expression SEMICOLON
		{ $$ = new AssignNode( $1, $3, $2.line,$2.column ); }
	| PRINT LPAREN printList RPAREN SEMICOLON
		{ $$ = new PrintNode( $3, $1.line,$1.column ); }
	| COUT AARROW coutList SEMICOLON
		{ $$ = new CoutNode( $3, $1.line, $1.column ); }
	| IF LPAREN expression RPAREN stmt %prec THEN
		{ $$ = new IfNode($3, $5, (StmtNode *)NULLnode, $2.line, $2.column); }
	| IF LPAREN expression RPAREN stmt ELSE stmt
		{ $$ = new IfNode($3, $5, $7, $2.line, $2.column); }
	| WHILE LPAREN expression RPAREN stmt
		{ $$ = new WhileNode($3, $5, $1.line, $1.column); }
	| LBRACE varList stmtList RBRACE
		{ $$ = new BlockNode($2, $3, $1.line, $1.column); }
	| RETURN SEMICOLON
		{ $$ = new ReturnNode((ExpressionNode *)NULLnode, $1.line, $1.column); }
	| RETURN expression SEMICOLON
		{ $$ = new ReturnNode($2, $1.line, $1.column); }
	| identifier LPAREN RPAREN SEMICOLON
		{ ArgListNode * n = (ArgListNode *)NULLnode;
		  $$ = new ProcCallNode($1, n, $2.line, $2.column); }
	| identifier LPAREN argList RPAREN SEMICOLON
		{ $$ = new ProcCallNode($1, $3, $2.line, $2.column); }
        ;

argList : argList COMMA expression
		{ $$ = new ArgListNode($1, $3, $1->line, $1->column); }
	| expression
		{ $$ = new ArgListNode((ArgListNode *)NULLnode, $1, $1->line, $1->column); }
	;

printList : printList COMMA printItem
		{ $$ = new PrintListNode($1, $3, $1->line, $1->column); }
	  | printItem
		{ PrintListNode * n = (PrintListNode *)NULLnode;
                  $$ = new PrintListNode(  n, $1, $1->line,$1->column); }
	  ;

printItem : stringLiteral { $$ = $1; } 
	  | expression    { $$ = $1; }
	  ;

coutList : coutList AARROW coutItem
		{ $$ = new CoutListNode($1, $3, $1->line, $1->column); }
	 | coutItem
		{ CoutListNode *n = (CoutListNode *)NULLnode;
		  $$ = new CoutListNode( n, $1, $1->line, $1->column); }
	 ;

coutItem : stringLiteral {$$ = $1; }
	 | expression    {$$ = $1; };

expression : ORexpression { $$ = $1; };

ORexpression: ORlist { $$ = $1; };

ORlist: ORlist OR ANDexpression
		{ $$ = new OrNode($1, $3, $1->line, $1->column); }
      | ANDexpression
		{ $$ = new OrNode((ExpressionNode *)NULLnode, $1, $1->line, $1->column); }
      ;

ANDexpression: ANDlist { $$ = $1; };

ANDlist: ANDlist AND term 
		{ $$ = new AndNode($1, $3, $1->line, $1->column); }
       | term
		{ $$ = $1; };
       

term	: factor LT  factor 
		{ $$ = new RelOpNode( LT, $1, $3, $2.line, $2.column); }
	| factor GT  factor
		{ $$ = new RelOpNode( GT, $1, $3, $2.line, $2.column); }
	| factor LEQ factor
		{ $$ = new RelOpNode( LEQ, $1, $3, $2.line, $2.column); }
	| factor GEQ factor
		{ $$ = new RelOpNode( GEQ, $1, $3, $2.line, $2.column); }
	| factor EQ  factor
		{ $$ = new RelOpNode( EQ, $1, $3, $2.line, $2.column); }
	| factor NEQ factor
		{ $$ = new RelOpNode( NEQ, $1, $3, $2.line, $2.column); }
	| factor 
		{ $$ = $1; }
	;

factor	: factor PLUS  primary
	  { $$ = new ArithOpNode( PLUS , $1, $3, $2.line,$2.column ); }
	| factor MINUS primary
	  { $$ = new ArithOpNode( MINUS, $1, $3, $2.line,$2.column ); }
	| primary 
	  { $$ = $1; }
	;

primary	: primary TIMES unit { $$ = new ArithOpNode( TIMES, $1, $3, $2.line, $2.column); }
	| primary SLASH unit { $$ = new ArithOpNode( SLASH, $1, $3, $2.line, $2.column); }
	| unit { $$ = $1; }
	;

unit : nameVar { $$ = $1; }
     | arrayVar { $$=$1;}
     | intLiteral { $$=$1;}
     | MINUS intLiteral { $$=$2; $$->myIntVal=-$2->myIntVal;}
     | identifier LPAREN RPAREN { $$= new FnCallNode($1, (ArgListNode *)NULLnode, $2.line, $2.column); }
     | identifier LPAREN argList RPAREN { $$= new FnCallNode($1, $3, $2.line, $2.column); }
     | TRUE { $$ = new TrueNode($1.line, $1.column);}
     | FALSE { $$ = new FalseNode($1.line, $1.column);}
     | LPAREN expression RPAREN { $$=$2; }
     ;

nameVar : identifier { $$ = new NameVarNode( $1, $1->line,$1->column ); };

arrayVar: identifier LBRACKET expression RBRACKET { $$ = new ArrayVarNode( $1, $3, $1->line, $1->column); };

intLiteral : INTLITERAL
             { $$ = new IntLiteralNode($1.intVal, $1.line,$1.column); };

stringLiteral : STRINGLITERAL { $$ = new StringLiteralNode($1.stringVal, $1.line, $1.column);};

target	: identifier
	            { ExpressionNode * n = (ExpressionNode *)NULLnode;
                      $$ = new TargetNode( $1,  n, $1->line,$1->column ); }
	| identifier LBRACKET expression RBRACKET
		    { $$ = new TargetNode($1, $3, $1->line, $1->column ); }
	;

identifier : ID { $$ = new IdentifierNode($1.stringVal, $1.line,$1.column); }
           ;

%%

extern char *yytext;
/* we must supply the yyerror function         */
/* bison demands int; yacc accepts int or void */
int yyerror( char *msg )
{
  cerr << "\nbison: " << msg
       << " line "    << yylval.t.line
       << " column "  << yylval.t.column
       << " (yytext=\""  << yytext << "\")\n\n";
  exit(1);
}
