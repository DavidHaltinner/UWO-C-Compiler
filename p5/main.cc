// main.cc

/*
 * Description:
 * 	This is a compiler for the language zinc
 *
 * Usage:
 * 	$ p5 <source_file>
 *	$ p5 -u <source_file>
 *	$ p5 -R <source_file>
 *	S p5 -u -R <source_file>
 *	$ p5 -R -u <source_file>
 *
 * Input:
 *	p5 takes between 1 and 3 arguments the last argument
 *	must always be the source file to be compiled, which must end
 *	in .z.  the flags -u and -R are optional. -u == unparse (or pretty 
 *	print info about the source file) and -R == Sethi Ullman Register 
 *	Allocation is to be used in code generation.
 *
 * Written by: 
 * 	Shawn Gens and David Haltinner 
 */

using namespace std;
#include <iostream>
#include <fstream>

#include "ast.h"
#include "scanner.h"   // yyin is here

ASTnode * astRoot;     // root of AST

extern int  declErrors;
extern int  typeErrors;
bool SethiUllmanFlag;
extern ofstream asmFile;

int main( int argc, char **argv )
{
  // Check for correct usage (arguments)
  if ( argc==1                                                         || 
       argc==3 && (strcmp(argv[1],"-u")!=0&&strcmp(argv[1],"-R")!=0)   ||
       argc==4 && ((strcmp(argv[1], "-u")!=0&&strcmp(argv[2], "-R")!=0)&&
                  (strcmp(argv[1], "-R")!=0&&strcmp(argv[2], "-u")!=0))||
       argc>4 ) {
    cerr << "\nusage: " << argv[0] << " <source_file>"    <<endl;
    cerr <<   "       " << argv[0] << " -u <source_file>" <<endl;
    cerr <<   "       " << argv[0] << " -R <source_file>" <<endl;
    cerr <<   "       " << argv[0] << " -u -R <source_file>" <<endl;
    cerr <<   "       " << argv[0] << " -R -u <source_file>" <<endl;
    cerr <<   "       options: -u==unparse, -R==SethiAllman register allocation"
         <<endl<<endl;
    return 1;
  }

  bool unparse       = false;
  SethiUllmanFlag    = false;
  int  fileName      = 1;

  // Which flags (if any) are set?
  if ( (argc==3||argc==4) && strcmp(argv[1],"-u")==0 ) {
    unparse  = true;
    fileName = 2;
  }
  if( (argc==3||argc==4) && strcmp(argv[1], "-R")==0 ) {
    SethiUllmanFlag = true;
    fileName        = 2;
  }
  if( argc==4 && strcmp(argv[2], "-R")==0 ) {
    SethiUllmanFlag = true;
    fileName        = 3;
  }
  if( argc==4 && strcmp(argv[2], "-u")==0 ) {
    unparse  = true;
    fileName = 3;
  }

  // Now look at the source file
  string file_name = argv[fileName];
  string asmfile = file_name;
  string ex = "s";
  if (file_name.size() <= 2)
  {
        cerr << endl << argv[0] << ": " << argv[1]
             << " does not have a \".z\" extension"
             << endl << endl;
        return 1;
  }
  if (file_name.substr(file_name.size()-2, 2) != ".z")
  {
        cerr << endl << argv[0] << ": " << argv[1]
             << " does not have a \".z\" extension"
             << endl << endl;
        return 1;
  }

  if ( (yyin = fopen(argv[fileName],"r")) == NULL ) {
    cerr<<endl<<argv[0] << ": could not open " << argv[fileName] <<endl<<endl;
    return 1;
  }
 
  asmfile[asmfile.length()-1] = ex[0];
  char afile[asmfile.length()];
  for(unsigned int i=0; i<=asmfile.length(); i++){
	afile[i] = asmfile[i];
  }

  asmFile.open(afile);
  if(asmFile.fail()){
	cerr << endl << argv[0] << ": could not open " << asmfile <<endl<<endl;
	return 1;
  }

  // Start compilation
  extern int yyparse();
  cout << "source file: " << argv[fileName] << endl;
  cout << "...parse" << endl;

  if ( yyparse() == 0 ) {

    cout << "...name check" << endl;
    astRoot->checkNames();
    if (declErrors>0) {
        string msg = "error";
        if (declErrors>1) msg = msg + "s";
        cerr << declErrors << " declaration/use " << msg << " found" << endl;
        cerr << "Compilation aborted." << endl;
        return 1;
    }

    cout << "...type check" << endl;
    astRoot->checkTypes();
    if (typeErrors>0) {
        string msg = "error";
        if (typeErrors>1) msg = msg + "s";
        cerr << typeErrors << " type " << msg << " found" << endl;
        cerr << "Compilation aborted." << endl;
        return 1;
    }

    // The file made it to code generation
    cout << "...emit code";
    if( SethiUllmanFlag ) {
	// Sethi Ullman Flag has been set...tell the user
	cout << " using Sethi Ullman register allocation";
    }
    cout << endl;
    astRoot->emitCode();

    if ( unparse ) {
	// The unparse flag is set...tell the user
	cout << "...unparse" << endl;
	astRoot->unparse();
    }
  }

  fclose(yyin);
  return 0;
}
