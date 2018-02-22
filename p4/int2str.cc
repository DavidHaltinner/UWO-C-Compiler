// int2str.cc

#include "int2str.h"
#include <assert.h>

//**********************************************************************
// IntToStr
//   convert the given (non-negative) int to a String
// **********************************************************************
string IntToStr(int k)
{ int digit;
  string tmp = "";

  assert(k >= 0);
  if (k == 0) return("0");
  while (k>0) {
    digit = k % 10;
    k = k / 10;
    switch (digit) {
    case 0:
      tmp = "0" + tmp;
      break;
    case 1:
      tmp = "1" + tmp;
      break;
    case 2:
      tmp = "2" + tmp;
      break;
    case 3:
      tmp = "3" + tmp;
      break;
    case 4:
      tmp = "4" + tmp;
      break;
    case 5:
      tmp = "5" + tmp;
      break;
    case 6:
      tmp = "6" + tmp;
      break;
    case 7:
      tmp = "7" + tmp;
      break;
    case 8:
      tmp = "8" + tmp;
      break;
    case 9:
      tmp = "9" + tmp;
      break;
    }
  }
  return(tmp);
}
