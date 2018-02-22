// unparse.cc

#include <assert.h>
#include <iomanip>
#include <typeinfo>
#include  "ast.h"
#include  "scanner.h"
#include  "y.tab.h"


void indent(int indentVal=0)
{
  for (int i=1; i<=indentVal; i++) cout << "   ";  // 3 spaces
}


//============================================================


void ProgramNode :: unparse( int indentVal )
{ 
  indent(indentVal+1);
  cout<<"functionName<parameters:locals:frameSize>;"<<endl;
  indent(indentVal+1);
  cout<<"variableName<offset>\n";
  myGlobalList   -> unparse();
  myFunctionList -> unparse();
}


void GlobalListNode :: unparse( int indentVal )
{
  myGlobalList -> unparse();
  myDecl       -> unparse();
}


void TypeIntNode :: unparse( int indentVal )
{
  cout << "int  ";
}

void TypeBoolNode :: unparse( int indentVal )
{
  cout << "bool ";
}

void VariableDeclNode :: unparse( int indentVal )
{
  doLineNum();
  indent(indentVal);
  myType       -> unparse(indentVal); 
  myIdentifier -> unparse(indentVal);
  if(myIdentifier->getSymbol()->adr == Local){
	  cout<<" <" << myIdentifier->getSymbol()->offset << "> "; 
  }
  cout << ";" << endl;
}


void FunctionDeclNode :: unparse( int indentVal )
{  
  doLineNum();
  myType -> unparse(indentVal);
  if(myType->isNull())
  {
    indent(indentVal);
    cout<<"void ";
  }
  myIdentifier -> unparse(indentVal);
  cout<<" <"<<mySymbolTableEntry->parameterCount<<":"
      <<mySymbolTableEntry->localVarCount<<":"<<mySymbolTableEntry->frameSize
      <<"> (";
  myParameters -> unparse(indentVal);
  cout << ")" << endl;
  
  doLineNum();
  cout << "{" << endl;
  myVarList->unparse(indentVal+1);
  myStmtList -> unparse(indentVal+1);
  doLineNum();
  cout << "}" << endl;
}

void ParametersNode:: unparse(int indentVal){
  myParamList -> unparse(indentVal);
}

void ParamListNode:: unparse(int indentVal){
  if(!myParamList->isNull()){
    myParamList->unparse(indentVal);
    cout<< ", ";
  }
  cout<<endl;
  doLineNum();
  indent(indentVal+2);
  myParam->unparse(indentVal);
}

void ParamValNode::unparse(int indentVal){
  myType->unparse(indentVal);
  myIdentifier->unparse(indentVal);
  cout << " <" << myIdentifier->getSymbol()->offset << "> ";
}

void ParamArrayNode::unparse(int indentVal){
  myType->unparse(indentVal);
  myIdentifier->unparse(indentVal);
  cout<< " <" << myIdentifier->getSymbol()->offset << "> " << "[]";
}

void ArgListNode :: unparse(int indentVal){
  if (!myArgList->isNull()) {
    myArgList->unparse(indentVal);
    cout<<", ";
  }
  myExpression->unparse(indentVal);
}

void OrNode :: unparse(int indentVal){ 
  if(!myLeft->isNull()){
    cout << "(";
    myLeft->unparse(indentVal);
    cout <<" || ";
    myRight->unparse(indentVal);
    cout << ")";
    return;
  }
  myRight->unparse(indentVal);
}

void StringLiteralNode::unparse(int indentVal){
   cout<< *myStringVal;
}

void FnCallNode::unparse(int indentVal){
  myIdentifier->unparse(indentVal);
  cout<<"(";
  myArgList->unparse(indentVal);
  cout<<")";
}

void TrueNode::unparse(int indentVal){
  cout<<"true";
}

void FalseNode::unparse(int indentVal){
  cout<<"false";
}

void ArrayVarNode::unparse(int indentVal){
  myIdentifier->unparse(indentVal);
  cout<<"[";
  myExpression->unparse(indentVal);
  cout<<"]";
}

void AndNode::unparse(int indentVal){
  if(!myLeft->isNull()){
    cout << "(";
    myLeft->unparse(indentVal);
    cout << " && ";
    myRight->unparse(indentVal);
    cout << ")";
    return;
  }
  myRight->unparse(indentVal);
}

void ReturnNode::unparse(int indentVal){
  doLineNum();
  indent(indentVal);
  cout<<"return";
  if(!myReturnVal->isNull()){
	cout << " ";
  	myReturnVal -> unparse(indentVal);
  }
  cout << ";" << endl;
}

void IfNode::unparse(int indentVal){
  doLineNum();
  indent(indentVal);
  cout<<"if (";
  myExpression->unparse(indentVal);
  cout<<")"<<endl;
  if(!myThenStmt->isBlock()){
    indentVal++;
  }
  myThenStmt->unparse(indentVal);
  if(!myElseStmt->isNull()){
    doLineNum();
    if(!myThenStmt->isBlock()){
      indentVal = indentVal - 1;
    }
    indent(indentVal);
    cout<<"else"<<endl;
    if(!myElseStmt->isBlock()){
      indentVal = indentVal + 1;
    }
    myElseStmt->unparse(indentVal);
  }
}

void BlockNode::unparse(int indentVal){
  doLineNum();
  indent(indentVal);
  cout<<"{"<<endl;
  if(!myVarList->isNull()){
	myVarList->unparse(indentVal+1);
  }
  myStmtList->unparse(indentVal+1);
  doLineNum();
  indent(indentVal);
  cout<<"}"<<endl;
}

void WhileNode::unparse(int indentVal){
  doLineNum();
  indent(indentVal);
  cout<<"while (";
  myExpression->unparse(indentVal);
  cout<<")"<<endl;
  if(!myStmt->isBlock()){
    indentVal = indentVal + 1;
  }
  myStmt->unparse(indentVal);
}

void VarListNode::unparse(int indentVal){
  if(!myVarList->isNull()){
    myVarList->unparse(indentVal);
  }
  myDecl->unparse(indentVal);
}

void ProcCallNode::unparse(int indentVal){
  doLineNum();
  indent(indentVal);
  myIdentifier->unparse(indentVal);
  cout<<"(";
  myArgList->unparse(indentVal);
  cout<<");"<<endl;
}

void ArrayDeclNode :: unparse( int indentVal ) {
  doLineNum();
  indent(indentVal);
  myType -> unparse(indentVal);
  myIdentifier -> unparse(indentVal);
  cout << " <" <<myIdentifier->getSymbol()->offset<< "> ";
  cout << "[";
  myIntLit -> unparse(indentVal);
  cout << "];"<< endl;
}

void FunctionListNode :: unparse( int indentVal ){
  myFunctionList -> unparse(indentVal);
  myFunctionDecl -> unparse(indentVal);
}

void StmtListNode :: unparse( int indentVal ){
  myStmtList -> unparse(indentVal);
  myStmt     -> unparse(indentVal);
}

void AssignNode :: unparse( int indentVal ){
  doLineNum();
  indent(indentVal);
  myTarget -> unparse( indentVal );
  cout << " = ";
  myExpression -> unparse( indentVal );
  cout << ";" << endl;
}

void PrintNode :: unparse( int indentVal ){
  doLineNum();
  indent(indentVal);
  cout<<"print(";
  myPrintList->unparse( indentVal );
  cout<<");"<<endl;
}

void PrintListNode :: unparse( int indentVal ){
  if(!myPrintList->isNull()){
    myPrintList->unparse( indentVal );
    cout << ",";
  }
  myPrintItem->unparse( indentVal );
  if (myPrintItem->nodeType==IntType) cout<<" <IntType>";
  else if (myPrintItem->nodeType==BoolType) cout<<" <BoolType>";
  else if (myPrintItem->nodeType==StringType) cout<<" <StringType>";
}

void CoutNode :: unparse( int indentVal ){
  doLineNum();
  indent(indentVal);
  cout<<"cout";
  myCoutList->unparse( indentVal );
  cout<<";"<<endl;
}

void CoutListNode :: unparse( int indentVal ){
  myCoutList->unparse( indentVal );
  cout << "<<";
  myCoutItem->unparse( indentVal );
}

void ArithOpNode :: unparse( int indentVal ) {
  cout << "(";
  myLeft -> unparse(indentVal);
  switch (myArithOp) {
	case PLUS  :  cout << " + ";  break;
	case MINUS :  cout << " - ";  break;
	case TIMES :  cout << " * ";  break;
	case SLASH :  cout << " / ";  break;
	default    :  assert(0);
  }
  myRight -> unparse(indentVal);
  cout << ")";
}

void RelOpNode :: unparse(int indentVal ){
  cout << "(";
  myLeft -> unparse(indentVal);
  switch (myRelOp) {
	case LT  :  cout << " < ";  break;
 	case GT  :  cout << " > ";  break;
	case EQ  :  cout << " == "; break;
	case NEQ :  cout << " != "; break;
	case LEQ :  cout << " <= "; break;
	case GEQ :  cout << " >= "; break;
	default  : assert(0);
  }
  myRight -> unparse(indentVal);
  cout << ")";
}

void NameVarNode :: unparse( int indentVal ){
  myIdentifier -> unparse(indentVal);
}


void IdentifierNode :: unparse( int indentVal ){
  cout << myStringVal->c_str();
}

void TargetNode :: unparse(int indentVal ){
  myIdentifier -> unparse(indentVal);
  if(!myExpression->isNull()){
    cout <<"[";
    myExpression -> unparse(indentVal);
    cout <<"]";
  }
}

void IntLiteralNode :: unparse( int indentVal ){
  cout << myIntVal;
}

