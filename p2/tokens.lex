/*
 * Description
 * This file is used by flex to identify tokens for the language zinc.
 *
 * Usage
 * % flex tokens.lex
 *
 * Input
 * No input
 * 
 * Written By
 * David Haltinner	Sep '03
*/

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
identifier	{letter}({letter}|{digit})*
slash		[/]
comment		{slash}{slash}(.)*{newline}
quote		["]
escape		[\\]
stringliteral	{quote}({letter}|{escape})(.)*{quote}
other		.

%{
/*
 *  stuff here is copied directly into lex.yy.c
 *  as info outside of (above) yylex()
 */

#include "message.h"
#include "scanner.h"
#include "y.tab.h"    // token definitions created by bison
#include <string>
#include <cstdlib>
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
{comment}	{ line++; column=1; }
"=="            { setPosition(); return EQ;        }
"!="            { setPosition(); return NEQ;       }
"<="            { setPosition(); return LEQ;       }
">="            { setPosition(); return GEQ;       }
"||"            { setPosition(); return OR;        }
"&&"            { setPosition(); return AND;       }
"="             { setPosition(); return ASSIGN;    }
"("             { setPosition(); return LPAREN;    }
")"             { setPosition(); return RPAREN;    }
"{"             { setPosition(); return LBRACE;    }
"}"             { setPosition(); return RBRACE;    }
"["             { setPosition(); return LBRACKET;  }
"]"             { setPosition(); return RBRACKET;  }
";"             { setPosition(); return SEMICOLON; }
","             { setPosition(); return COMMA;     }
"+"             { setPosition(); return PLUS;      }
"-"             { setPosition(); return MINUS;     }
"*"             { setPosition(); return TIMES;     }
"/"             { setPosition(); return SLASH;     }
"<"             { setPosition(); return LT;        }
">"             { setPosition(); return GT;        }

{integer}	{
		  doIntConvert();
		  setPosition();
		  return INTLITERAL;
		}
{stringliteral} {
		  string c(yytext);
		  yylval.t.stringVal=new string(c);
		  setPosition();
		  return STRINGLITERAL;
		}
{identifier}	{
		  string c(yytext);
		  setPosition();
		  return idScreener(new string(c));
		}
{other}		{
		  string c(yytext);
		  string msg = "yylex(): ignoring illegal character ";
		  msg = msg + "\"" + c + "\".";
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
  else if ( *lexeme == "int"    ) return INT;
  else if ( *lexeme == "void"   ) return VOID;
  else if ( *lexeme == "return" ) return RETURN;
  else if ( *lexeme == "bool"   ) return BOOL;
  else if ( *lexeme == "true"   ) return TRUE;
  else if ( *lexeme == "false"  ) return FALSE;
  else if ( *lexeme == "if"     ) return IF;
  else if ( *lexeme == "else"   ) return ELSE;
  else if ( *lexeme == "while"  ) return WHILE;
  else {
	yylval.t.stringVal=lexeme;
	return ID;
  }
}

void doIntConvert()
{
  if (std::atoi(yytext)>MAXINT) {
    yylval.t.intVal=MAXINT;
    string msg="int literal exceeds MAXINT;\n\tsubstituting MAXINT";
    Warn(line,column, msg);
  }
  else yylval.t.intVal=std::atoi(yytext);  
}
