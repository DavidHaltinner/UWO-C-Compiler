// main.cc

// Written by Shawn Gens and David Haltinner
// 

#include <iostream>

#include "ast.h"
#include "scanner.h"   // yyin is here

ASTnode * astRoot;     // root of AST


int main( int argc, char **argv )
{
  if ( argc==1 || argc==3 && strcmp(argv[1],"-u")!=0 || argc>3 ) {
    cerr << "\nusage: " << argv[0] << " <source_file>"    <<endl;
    cerr <<   "       " << argv[0] << " -u <source_file>" <<endl;
    cerr <<   "       option: -u==unparse" <<endl<<endl;
    return 1;
  }

  bool unparse  = false;
  int  fileName = 1;
  if ( argc==3 && strcmp(argv[1],"-u")==0 ) {
    unparse  = true;
    fileName = 2;
  }

  string file_name = argv[fileName];
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


  extern int yyparse();
  cout << "source file: " << argv[fileName] << endl;
  cout << "...parse\n";

  if ( yyparse() == 0 ) {

	if ( unparse ) {
	    cout << "...unparse" << endl;
	    astRoot->unparse();
	}

  }

  fclose(yyin);
  return 0;
}
