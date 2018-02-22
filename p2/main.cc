/* 
 * Description
 * It reads in the text contents of a file, and turns the contents into token 
 * types that are used by flex. It then will output the token type and the row
 * and column that the token is found.
 *
 * Usage
 * % p2 testFile.z
 *
 * Input
 * The file name must contain a .z extension. The file will contain any 
 * characters, symbols, numbers, etc that are desired. Anything with // in
 * front will be considered a comment. Anything with quotes around it will be 
 * a string literal. The rest will be either reserved words or identifiers.
 * 
 * Written By
 * David Haltinner	Sep '03
 */

#include <iostream>
#include <iomanip>
#include <cassert>
#include "scanner.h"
#include "y.tab.h"   //token kinds created by bison (cf tokenKinds.h)
using namespace std;

YYSTYPE yylval;      //YYSTYPE defined in y.tab.h
extern int yylex();  //defined in lex.yy.c

int main( int argc, char **argv )
{
  if (argc != 2) {
	cerr<<"usage: "<<argv[0]<<" fileName"<<endl;
	return 1;
  }
  char *ext=(argv[1]+strlen(argv[1])-2);
  if ((*ext!='.') || (*(ext+1)!='z')) {
        cerr<<endl<<argv[0]<<": "<<argv[1]
            <<" does not have a \".z\" extension\n\n";
        return 1;
  }
  if ((yyin=fopen(argv[1],"r"))==NULL) {
	cerr<<endl<<argv[0]<<": could not open "<<argv[1]<<".\n\n";
	return 1;
  }
  int tokenKind = yylex();
  while (tokenKind) {
     cout << setw(2)<<yylval.t.line   << ":"
          << setw(2)<<yylval.t.column << "  ";
     cout << setw(15);
     switch (tokenKind) {
	case ASSIGN:
		cout << "ASSIGN" << endl;
		break;
	case SEMICOLON:
		cout << "SEMICOLON" << endl;
		break;
	case RPAREN:
		cout << "RPAREN" << endl;
		break;
	case RBRACE:
		cout << "RBRACE" << endl;
		break;
	case RBRACKET:
		cout << "RBRACKET" << endl;
		break;
	case INTLITERAL:
		cout << "INTLITERAL" << "  " << yylval.t.intVal << endl;
		break;
	case STRINGLITERAL:
		cout << "STRINGLITERAL" << "  " << yylval.t.stringVal->c_str() 
		     << endl;
		break;
	case PRINT:
		cout << "PRINT" << endl;
		break;
	case INT:
		cout << "INT" << endl;
		break;
	case VOID:
		cout << "VOID" << endl;
		break;
	case TRUE:
		cout << "TRUE" << endl;
		break;
	case FALSE:
		cout << "FALSE" << endl;
		break;
	case LPAREN:
		cout << "LPAREN" << endl;
		break;
	case LBRACE:
		cout << "LBRACE" << endl;
		break;
	case LBRACKET:
		cout << "LBRACKET" << endl;
		break;
	case PLUS:
		cout << "PLUS" << endl;
		break;
	case MINUS:
		cout << "MINUS" << endl;
		break;
	case TIMES:
		cout << "SLASH" << endl;
		break;
	case BOOL:
		cout << "BOOL" << endl;
		break;
	case AND:
		cout << "AND" << endl;
		break;
	case OR:
		cout << "OR" << endl;
		break;
	case IF:
		cout << "IF" << endl;
		break;
	case ELSE:
		cout << "ELSE" << endl;
		break;
	case WHILE:
		cout << "WHILE" << endl;
		break;
	case RETURN:
		cout << "RETURN" << endl;
		break;
	case LT:
		cout << "LT" << endl;
		break;
	case GT:
		cout << "GT" << endl;
		break;
	case LEQ:
		cout << "LEQ" << endl;
		break;
	case GEQ:
		cout << "GEQ" << endl;
		break;
	case EQ:
		cout << "EQ" << endl;
		break;
	case NEQ:
		cout << "NEQ" << endl;
		break;
	case COMMA:
		cout << "COMMA" << endl;
		break;
	case ID:
		cout << "ID" << "  " << yylval.t.stringVal->c_str() << endl;
		break;
	default: assert(0);
     }
     tokenKind = yylex();
  }
  return 0;
}
