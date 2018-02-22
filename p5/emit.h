// emit.h

#ifndef EMIT_H_
#define EMIT_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

int   getReg();
void freeReg( int r );
int   getArgReg();
void freeArgReg( int r );

void saveRegs();
void restoreRegs();

// ============================================================================

void emit( string opcode,
           string arg1="", string arg2="", string arg3="", string comment="" );

void emit( string opcode, int    rD, string arg );
void emit( string opcode, string rD, int    arg );
void emit( string opcode, int    rD, int    arg );

// ============================================================================

void emitOffset
    ( string opcode, string arg1, int offset, string arg2, string comment="" );
void emitOffset
    ( string opcode, int rD, int offset, string arg2, string comment="" );

// ============================================================================

void emitLabel( string label );
void emitComment     ( string comment );
void emitBlockComment( string comment );
void emitEmptyLine ();

// ============================================================================

string NextLabel();

#endif
