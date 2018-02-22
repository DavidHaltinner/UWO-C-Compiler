/* scanner.h */

#ifndef SCANNER_H_
#define SCANNER_H_

#include <stdio.h>
#include <iostream>
#include <string>
using std::string;
//input file pointer used by yylex();  defined in lex.yy.c
extern FILE *yyin;


struct TokenType {
  int    line;
  int    column;
  int    intVal;
  string *stringVal;
};

#endif
