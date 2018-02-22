// message.cc

#include <iostream>
#include <iomanip>
#include "message.h"

// GLOBAL VARIABLE
bool errorFlag = false;

// **********************************************************************
// Error
//    write the given error message, preceded by <line>:<col> to cerr
//    also set the global errorFlag to true
// **********************************************************************
void Error(int line, int col, string msg)
{
  cerr << "ERROR line " << setw(2) << line
       << " column " << setw(2) << col << ": " << msg << endl;
  errorFlag = true;
}

// **********************************************************************
// Warn
//    write the given warning message, preceded by <line>:<col> to cerr
// **********************************************************************
void Warn(int line, int col, string msg)
{
  cerr << "WARNING line " << setw(2) << line
       << " column " << setw(2) << col << ": " << msg << endl;
}

// **********************************************************************
// InternalError
//   write the given error message to cerr and quit
// **********************************************************************
void InternalError(string msg)
{
  cerr << endl << "INTERNAL COMPILER ERROR: " << msg << endl<<endl;
  exit(1);
}
