// emit.cc

using namespace std;
#include "ast.h"
#include "emit.h"
#include "int2str.h"
#include "message.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <assert.h>

ofstream asmFile;


//============================================================================
//
//                             Reg Routines
//
//============================================================================


// oops. should have 10 $t registers
static int regInUse[8] = { 0,0,0,0,0,0,0,0 }; // 0 == free; 1 == in use
static int nRegInUse = 0;

static int argRegInUse[4] = { 0,0,0,0 };      // 0 == free; 1 == in use
static int nArgRegInUse = 0;

int getReg()
{
  int i;

  for (i=0; i<8 && regInUse[i]==1; i++) {};
  if ( i==8 ) InternalError("emit: out of registers in getReg");

  regInUse[i] = 1;
  nRegInUse++;
  return i;
}

void freeReg( int r )
{
  if (r==30) {  return; }
  assert( r>=0 && r<=7 );
  regInUse[r] = 0;
  nRegInUse--;
}

static void emitOffsetSP
    ( string opcode, int rD, int offset )
{
  asmFile << "\t" << opcode << "\t" ;
  asmFile << "$t"+IntToStr(rD) << ", " << IntToStr(offset) << "($sp)" <<endl;
  asmFile.flush();
}

void saveRegs()
{
   if(nRegInUse>0) emit( "subu",  "$sp, $sp", IntToStr(nRegInUse*4) );
   for (int i=0; i<8; i++) if(regInUse[i]==1) emitOffsetSP("sw", i, i*4 );
}

void restoreRegs()
{
   for (int i=0; i<8; i++) if(regInUse[i]==1) emitOffsetSP("lw", i, i*4 );
   if(nRegInUse>0) emit( "addu",  "$sp, $sp", IntToStr(nRegInUse*4) );
}

int getArgReg()
{
  int i;

  for (i=0; i<4 && argRegInUse[i]==1; i++) {};
  if ( i==4 ) InternalError("emit: out of registers in getArgReg");

  argRegInUse[i] = 1;
  nArgRegInUse++;
  return i;
}

void freeArgReg( int r )
{
  assert( r>=0 && r<=3 );
  argRegInUse[r] = 0;
  nArgRegInUse--;
}


// ===========================================================================
//
//                             Emit Routines
//
// ===========================================================================


void emit( string opcode,
           string arg1, string arg2, string arg3,
           string comment )
{
  asmFile << "\t" << opcode;

  if ( arg1 != "" ) {
	asmFile << "\t" << arg1;
	if ( arg2 != "" ) {
		asmFile << ", " << arg2;
		if ( arg3 != "" ) {
			asmFile << ", " << arg3;
			if ( comment != "" ) asmFile << "\t" << comment;
		}
	}
  }

  asmFile<<endl;
  asmFile.flush();
}

// ===========================================================================

void emit( string opcode, int rD, string arg )
{
  asmFile << "\t" << opcode << "\t";

  asmFile << "$t" << IntToStr(rD) << ", " << arg << endl;
  asmFile.flush();
}

void emit( string opcode, string rD, int arg )
{
  asmFile << "\t" << opcode << "\t";

  asmFile << rD << ", " << arg << endl;
  asmFile.flush();
}

void emit( string opcode, int rD, int arg )
{
  asmFile << "\t" << opcode << "\t";

  asmFile << "$t" << IntToStr(rD) << ", " << arg << endl;
  asmFile.flush();
}

// ===========================================================================

void emitOffset
    ( string opcode, string arg1, int offset, string arg2, string comment )
{
  asmFile << "\t" << opcode << "\t" ;

  string stringOffset = (offset==0) ? IntToStr(offset) : "-"+IntToStr(offset);
  asmFile << arg1 << ", " << stringOffset << "(" << arg2 << ")";

  if (comment!="") asmFile << "\t" << comment;

  asmFile<<endl;
  asmFile.flush();
}

void emitOffset
    ( string opcode, int rD, int offset, string arg2, string comment )
{
  asmFile << "\t" << opcode << "\t" ;

  string stringOffset = (offset==0) ? IntToStr(offset) : "-"+IntToStr(offset);
  asmFile << "$t"+IntToStr(rD) << ", " << stringOffset << "(" << arg2 << ")";

  if (comment!="") asmFile << "\t" << comment;

  asmFile<<endl;
  asmFile.flush();
}

// ===========================================================================

void emitComment( string comment )
{
  asmFile << "#" << comment << endl;
  asmFile.flush();
}

void emitBlockComment( string comment )
{
  asmFile << "\t#####"    << endl;
  asmFile << "\t####### " << comment << " #######" << endl;
  asmFile << "\t#####"    << endl;
  asmFile.flush();
}

void emitEmptyLine()
{
  asmFile << endl;
  asmFile.flush();
}

// ===========================================================================

void emitLabel( string label )
{
  asmFile << label << ":" << endl;
}


// ===================================
// Return a different label each time:
//        .L0, .L1, .L2,    etc.
// ===================================
string NextLabel()
{
  static int currLabel = 0;

  string tmp = ".L" + IntToStr(currLabel++);
  return(tmp);
}
