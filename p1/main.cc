/*****************************************************************************
 * Program One
 * This program tests out the SymbolTable and Symbol objects by reading in
 * input from a file, and adding un-used words into the SymbolTable. It then
 * Outputs the words, their length and whether they were added to the console,
 * and outputs the words to a file named table.dump.
 * 
 * Usage
 * % ./p1 fileName
 *
 * Input
 * A text data file containing words.
 *
 * Written By
 * David Haltinner
 ****************************************************************************/
 
#include <iostream>
#include <iomanip>
#include <fstream>
#include "symboltable.h"
#include "symbol.h"
using namespace std;

int main(int argc, char* argv[])
{
  if (argc!=2) {                        //Make sure they gave me a file to use
    cout<<endl<<"usage: p1 fileName"<<endl<<endl;
    return 0;
  }
  ifstream in(argv[1], ios::in);         //Open the file
  if (!in) {                             //In case they gave me a bad file name
    cout<<endl<<"p1: unable to open "<<argv[1]<<endl<<endl;
    return 0;
  }
  SymbolTable * symbolTable=new SymbolTable();  //Make the table
  string temp;                                //Use to hold the inputted string
  cout<<"string"<<setw(9)<<"len"<<setw(11)<<"entered?"<<endl<<endl;
  SymbolPtr tempSymbol;                     //Used to make the inserted symbols
  while (in>>temp) {                     //While their is data left in the file
    cout<<setiosflags(ios::left)<<setw(8);  //setup spacing for 7
    if(temp.length()>7) cout<<temp.substr(0,7); //if more than 7
    else cout<<temp;                            //if less than 7
    cout<<setw(7)<<resetiosflags(ios::left)<<temp.length(); //rest
    if (symbolTable->Lookup(temp)==NULL) {               //Is it already there?
      tempSymbol=new Symbol(temp);                       //Make the new symbol
      symbolTable->Insert(tempSymbol);                   //Add the new word
      cout<<setw(9)<<resetiosflags(ios::left)<<"yes"<<endl;  
    }
    else cout<<setw(9)<<resetiosflags(ios::left)<<"no"<<endl;
  }
  fstream out("table.dump", ios::out);       //Open the output file
  symbolTable->Print(out);                   //Dump the SymbolTable to the file
  out.close();                               //Close the output file
  out.clear();                               //Clear the File Flags
  in.close();                                //Close the input file
  in.clear();                                //Clear the File Flags
}
