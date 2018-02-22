/* tokens.lex */

/*
 *  char *yytext  contains the lexeme of each token
 *  int   yyleng  is the length of the lexeme
 */

/*
 *  section 1:  names for regular expressions
 */

delimiter	[ \t]
newline		[\n]
whitespace	{delimiter}+
digit		[0-9]
letter		[a-zA-Z]
integer		{digit}+
other		.
identifier 	({letter}|_)({letter}|{digit}|_)*
comment1	"//"[^\n]*[\n]
comment2	"//"[^\n]*
mgcStrLt	\"([^\"\n]|\\[^\n])*\"
strLt		\"([^\"\000-\037\177\n\\]|\\[^\000-\037\177\n])*\"
rnAwyStrLt	\"([^\"\000-\037\177\n]|\\[^\000-\037\177\n])*

%{
/*
 *  stuff here is copied directly into lex.yy.c
 *  as info outside of (above) yylex()
 */
using namespace std;
#include "ast.h"
#include "message.h"
#include "scanner.h"
#include "y.tab.h"    // token definitions created by bison
#include "int2str.h"

#define  MAXINT  2147483647

static int line   = 1;  /* current line   number */
static int column = 1;  /* current column number */

/* forward */
void setPosition();
void doIntConvert();
int  idScreener(string *lexeme);

%}

%%

%{
/*
 * section 2: rules for regular expressions
 */
%}

{whitespace}	{ column += yyleng; }
{newline}	{ line++; column=1; }

"="             { setPosition(); return ASSIGN;    }
","		{ setPosition(); return COMMA;	   }
";"		{ setPosition(); return SEMICOLON;  }
"("             { setPosition(); return LPAREN;    }
")"		{ setPosition(); return RPAREN;    }
"{"             { setPosition(); return LBRACE;    }
"}"		{ setPosition(); return RBRACE;	   }
"["             { setPosition(); return LBRACKET;  }
"]"		{ setPosition(); return RBRACKET;  }
"+"		{ setPosition(); return PLUS;	   }
"-"		{ setPosition(); return MINUS;	   }
"*"		{ setPosition(); return TIMES;	   }
"/"		{ setPosition(); return SLASH;	   }
">"		{ setPosition(); return GT;	   }
"<"		{ setPosition(); return LT;	   }
"=="		{ setPosition(); return EQ;	   }
"!="	        { setPosition(); return NEQ;	   }
">="		{ setPosition(); return GEQ; 	   }
"<="		{ setPosition(); return LEQ;	   }
"&&"		{ setPosition(); return AND;	   }
"||"		{ setPosition(); return OR;	   }
"<<"		{ setPosition(); return AARROW;    }

{integer}	{
		  doIntConvert();
		  setPosition();
		  return INTLITERAL;
		}

{identifier}	{
		  setPosition();
		  string *lexeme = new string(yytext);
		  yylval.t.stringVal = lexeme;
		  return idScreener(lexeme);
		}

{strLt}		{
		  setPosition();
		  yylval.t.stringVal = new string(yytext);
		  return STRINGLITERAL;		  	
		}

{rnAwyStrLt}	{
		  string c(yytext);
		  string msg = "unterminated string ";
		  msg = msg  + c;
		  Warn(line,column, msg);
		  column += yyleng;
		}

{mgcStrLt}	{
		  string c(yytext);
		  string msg = "magic string ";
		  msg = msg + c;
		  Warn(line,column, msg);
		  column += yyleng;
		}

{comment1}	{ line++; column = 1; }
{comment2}	{ line++; column = 1; }

{other}		{
		  int bad = yytext[0];
		  string c(IntToStr(bad));
		  string msg = "yylex(): ignoring illegal character ";
		  msg = msg + " (ascii code " + c + ")";
		  Warn(line,column, msg);
		  column += yyleng;
		}

%%

/*
 *  section 3: functions used in section 2
 */

void setPosition()
{
  yylval.t.line   = line  ;
  yylval.t.column = column;
  column += yyleng;
}

int idScreener(string *lexeme)
{
  if      ( *lexeme == "print"  ) return PRINT;
  if      ( *lexeme == "cout"   ) return COUT;
  else if ( *lexeme == "int"    ) return INT;
  else if ( *lexeme == "void"   ) return VOID;
  else if ( *lexeme == "if"	) return IF;
  else if ( *lexeme == "else"   ) return ELSE;
  else if ( *lexeme == "while"  ) return WHILE;
  else if ( *lexeme == "bool"   ) return BOOL;
  else if ( *lexeme == "true"   ) return TRUE;
  else if ( *lexeme == "false"  ) return FALSE;
  else if ( *lexeme == "return" ) return RETURN;
  else                            return ID;
}

void doIntConvert()
{
	if(atof(yytext) > MAXINT){
		yylval.t.intVal = MAXINT;
		string msg = 
			"int literal exceeds MAXINT;\n\tsubstituting MAXINT";
		Warn(line, column, msg);
	}
	else{
		yylval.t.intVal = atoi(yytext);
	}
}
