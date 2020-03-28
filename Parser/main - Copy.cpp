#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

#include <vector>
#include<fstream>

using namespace std;



int x=0;   //current index of parsing


ifstream fin("input.txt");
/////////////////////////////////////
struct tokenobj
{
    int id;
    string name;
    string value;
};
vector<tokenobj> objarray;
struct tokenobj tempTok;
////////////////////////////////////////
enum id {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,
  tok_LParanteses = -6,
  tok_RParanteses = -7,
  tok_LBracket= -8,
  tok_RBracket = -9,
  tok_asign = -10,
  data_type=-11,
  tok_for=-12,
  data_arg_array=-13,
  tok_choose=-14,
  tok_arithmetic_oper=-15,
  tok_in=-16,
  ////////-17
  tok_return=-18,
  tok_property=-20,
  tok_logic_oper=-21,
  tok_main= -22
};

string my_map[]={"tok_eof","tok_def", "tok_extern", "tok_identifier","tok_number","tok_LParanteses", "tok_RParanteses",  "tok_LBracket",  "tok_RBracket",  "tok_asign", "data_type",  "tok_for",
"data_arg_array",
"tok_choose",
 "tok_arithmetic_oper",
"tok_in",
"tok_return",
"tok_property",
 "tok_logic_oper",
 "tok_main"
 };

static string IdentifierStr;

static double NumVal;
int parStack=0;
int brStack=0;
bool wasMain=false;
string intermediar;
//////////////////////////////////////////
static int gettok() {
  static int LastChar = ' ';

  // Skip any whitespace.
  while (isspace(LastChar))
    LastChar = fin.get();
    if (isalpha(LastChar) || LastChar=='#') { // identifier: [a-zA-Z][a-zA-Z0-9]*
  IdentifierStr = LastChar;
  while (isalnum((LastChar = fin.get()))|| LastChar=='_')
    IdentifierStr += LastChar;
tempTok.value=IdentifierStr;
  if (IdentifierStr == "def")
    return tok_def;
  if (IdentifierStr == "extern")
    return tok_extern;
  if (IdentifierStr == "int")
    return data_type;
    if (IdentifierStr == "float")
    return data_type;
    if (IdentifierStr == "double")
    return data_type;
    if (IdentifierStr == "#arg")
    return data_arg_array;
    if (IdentifierStr == "main"){
        wasMain=true;
        return tok_main;}
    if (IdentifierStr == "for")
    return tok_for;
    if (IdentifierStr == "in")
    return tok_in;
    if (IdentifierStr == "return")
    return tok_return;
    return tok_identifier;
}
if (isdigit(LastChar) || LastChar == '.') {   // Number: [0-9.]+
  string NumStr;
  do {
    NumStr += LastChar;
    LastChar = fin.get();
  } while (isdigit(LastChar) || LastChar == '.');
    tempTok.value=NumStr;
  NumVal = strtod(NumStr.c_str(), 0);
  if (NumStr==".")
    return tok_property;
  return tok_number;
}
if (LastChar == '$') {
  do
    LastChar = fin.get();
  while (LastChar != '\0' && LastChar != '\n' && LastChar != '\r');

  if (LastChar != '\0')
    return gettok();
}
intermediar=LastChar;
tempTok.value=intermediar;
/////////////////////////////////L and R parantheses

if (LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/') {
  LastChar=fin.get();
    return tok_arithmetic_oper;
}
if (LastChar == '<' || LastChar == '>' || LastChar == '~') {
  LastChar=fin.get();
  if(LastChar!='=')
    return tok_logic_oper;
    string chars;
    chars+=LastChar;
    chars+='=';
   tempTok.name= chars;
    LastChar=fin.get();
    return tok_logic_oper;
}
if ( LastChar == '=') {
  LastChar=fin.get();
  if(LastChar!='=')
    return tok_asign;
   tempTok.value= "==";
    LastChar=fin.get();
    return tok_logic_oper;
}
/////////////////////////////////Comments
if (LastChar == '$') {
  do
    LastChar = fin.get();
  while (LastChar != '\0' && LastChar != '\n' && LastChar != '\r');

  if (LastChar != '\0')
    return gettok();
}
  if (LastChar == '\0')
    return tok_eof;

return LastChar;
//  // Otherwise, just return the character as its ascii value.
  int ThisChar = LastChar;
  LastChar = fin.get();
  return ThisChar;
}

//===----------------------------------------------------------------------===//
// Abstract Syntax Tree (aka Parse Tree)
//===----------------------------------------------------------------------===//
namespace {
/// ExprAST - Base class for all expression nodes.
class ExprAST {
public:
  virtual ~ExprAST() {}
};

vector<ExprAST> exparr;

/// NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST : public ExprAST {
public:
  NumberExprAST(double val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST : public ExprAST {
  std::string Name;
public:
  VariableExprAST(const std::string &name) : Name(name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) {}
};

/// CallExprAST - Expression class for function calls.
class CallExprAST : public ExprAST {
  std::string Callee;
  std::vector<ExprAST*> Args;
public:
  CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
    : Callee(callee), Args(args) {}
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST {
  std::string Name;
  std::vector<std::string> Args;
public:
  PrototypeAST(const std::string &name, const std::vector<std::string> &args)
    : Name(name), Args(args) {}

};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
public:
  FunctionAST(PrototypeAST *proto, ExprAST *body) {}
};
} // end anonymous namespace

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() {
  return CurTok = objarray[x++].id;
}

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
  if (!isascii(CurTok))
    return -1;

  // Make sure it's a declared binop.
  int TokPrec = BinopPrecedence[CurTok];
  if (TokPrec <= 0) return -1;
  return TokPrec;
}

/// Error* - These are little helper functions for error handling.
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }

static ExprAST *ParseExpression();

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() {
  std::string IdName = IdentifierStr;

  getNextToken();  // eat identifier.

  if (CurTok != '(') // Simple variable ref.
    return new VariableExprAST(IdName);

  // Call.
  getNextToken();  // eat (
  std::vector<ExprAST*> Args;
  if (CurTok != ')') {
    while (1) {
      ExprAST *Arg = ParseExpression();
      if (!Arg) return 0;
      Args.push_back(Arg);

      if (CurTok == ')') break;

      if (CurTok != ',')
        return Error("Expected ')' or ',' in argument list");
      getNextToken();
    }
  }

  // Eat the ')'.
  getNextToken();

  return new CallExprAST(IdName, Args);
}

/// numberexpr ::= number
static ExprAST *ParseNumberExpr() {
  ExprAST *Result = new NumberExprAST(NumVal);
  getNextToken(); // consume the number
  return Result;
}

/// parenexpr ::= '(' expression ')'
static ExprAST *ParseParenExpr() {
  getNextToken();  // eat (.
  ExprAST *V = ParseExpression();
  if (!V) return 0;

  if (CurTok != ')')
    return Error("expected ')'");
  getNextToken();  // eat ).
  return V;
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static ExprAST *ParsePrimary() {
  switch (CurTok) {
  default: return Error("unknown token when expecting an expression");
  case tok_identifier: return ParseIdentifierExpr();
  case tok_number:     return ParseNumberExpr();
  case '(':            return ParseParenExpr();
  }
}

/// binoprhs
///   ::= ('+' primary)*
static ExprAST *ParseBinOpRHS(int ExprPrec, ExprAST *LHS) {
  // If this is a binop, find its precedence.
  while (1) {
    int TokPrec = GetTokPrecedence();

    // If this is a binop that binds at least as tightly as the current binop,
    // consume it, otherwise we are done.
    if (TokPrec < ExprPrec)
      return LHS;

    // Okay, we know this is a binop.
    int BinOp = CurTok;
    getNextToken();  // eat binop

    // Parse the primary expression after the binary operator.
    ExprAST *RHS = ParsePrimary();
    if (!RHS) return 0;

    // If BinOp binds less tightly with RHS than the operator after RHS, let
    // the pending operator take RHS as its LHS.
    int NextPrec = GetTokPrecedence();
    if (TokPrec < NextPrec) {
      RHS = ParseBinOpRHS(TokPrec+1, RHS);
      if (RHS == 0) return 0;
    }

    // Merge LHS/RHS.
    LHS = new BinaryExprAST(BinOp, LHS, RHS);
  }
}

/// expression
///   ::= primary binoprhs
///
static ExprAST *ParseExpression() {
  ExprAST *LHS = ParsePrimary();
  if (!LHS) return 0;

  return ParseBinOpRHS(0, LHS);
}

/// prototype
///   ::= id '(' id* ')'
static PrototypeAST *ParsePrototype() {
  if (CurTok != tok_identifier)
    return ErrorP("Expected function name in prototype");

  std::string FnName = IdentifierStr;
  getNextToken();

  if (CurTok != '(')
    return ErrorP("Expected '(' in prototype");

  std::vector<std::string> ArgNames;
  while (getNextToken() == tok_identifier)
    ArgNames.push_back(IdentifierStr);
  if (CurTok != ')')
    return ErrorP("Expected ')' in prototype");

  // success.
  getNextToken();  // eat ')'.

  return new PrototypeAST(FnName, ArgNames);
}

/// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {
  getNextToken();  // eat def.
  PrototypeAST *Proto = ParsePrototype();
  if (Proto == 0) return 0;

  if (ExprAST *E = ParseExpression())
    return new FunctionAST(Proto, E);
  return 0;
}

/// toplevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr() {
  if (ExprAST *E = ParseExpression()) {
    // Make an anonymous proto.
//    exparr.push_back(E);
    PrototypeAST *Proto = new PrototypeAST("", std::vector<std::string>());
    return new FunctionAST(Proto, E);
  }
  return 0;
}

/// external ::= 'extern' prototype
static PrototypeAST *ParseExtern() {
  getNextToken();  // eat extern.
  return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
  if (ParseDefinition()) {
    fprintf(stderr, "Parsed a function definition.\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleExtern() {
  if (ParseExtern()) {
    fprintf(stderr, "Parsed an extern\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

static void HandleTopLevelExpression() {
  // Evaluate a top-level expression into an anonymous function.
  if (ParseTopLevelExpr()) {
    fprintf(stderr, "Parsed a top-level expr\n");
  } else {
    // Skip token for error recovery.
    getNextToken();
  }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
  while (1) {
    fprintf(stderr, "ready>%d ",CurTok); //prints the ID of the token that will be handled
    switch (CurTok) {
    case tok_eof:    return;
    case ';':        getNextToken(); break;  // ignore top-level semicolons.
    case tok_def:    HandleDefinition(); break;
    case tok_extern: HandleExtern(); break;
    default:         HandleTopLevelExpression(); break;
    }
  }
}

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//

int main()
{
  BinopPrecedence['<'] = 10;
  BinopPrecedence['>'] = 10;
  BinopPrecedence['+'] = 20;
  BinopPrecedence['-'] = 20;
  BinopPrecedence['*'] = 40;  // highest.
  BinopPrecedence['/'] = 40;  // highest.
    int tok;

    while(!fin.eof()){
    tok=gettok();
  //  cout<<tok<<endl;
   tempTok.id= tok;
    tempTok.name = my_map[-tok-1];
    objarray.push_back(tempTok);
    }
    tempTok.id=tok_eof;
    tempTok.name = my_map[-tok_eof-1];
    tempTok.value="EOF";
    objarray.push_back(tempTok);

    for (unsigned i = 0; i < objarray.size(); i++) {
	cout <<"id: "<<objarray[i].id<<"\t name: "<< objarray[i].name << "\t = \'" +objarray[i].value <<"\'"<< '\n';
}
getNextToken();
MainLoop();
    return 0;
}

