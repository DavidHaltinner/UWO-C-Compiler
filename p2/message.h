// message.h

#ifndef MESSAGE_GUARD
#define MESSAGE_GUARD

#include <string>
using std::string;

void Error(int line, int col, string msg);
void Warn (int line, int col, string msg);
void InternalError(string msg);

// GLOBAL VARIABLE
extern bool errorFlag;

#endif
