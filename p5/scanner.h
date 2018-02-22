/* scanner.h */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

//input file pointer used by yylex();  defined in lex.yy.c
extern FILE *yyin;


struct TokenType {
  int    line;
  int    column;
  int    intVal;     // used by integer token
  string *stringVal; // used by identifier and string tokens
};

#endif
