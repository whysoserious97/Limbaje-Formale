#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include<fstream>

using namespace std;
ifstream fin("input.txt");

enum Token {
  tok_eof = -1,

  // commands
  tok_func = -2,
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
  tok_endoperation=-17,
  tok_return=-18,
  tok_delimiter=-19,
  tok_property=-20,
  tok_logic_oper=-21,
  tok_main= -22
};

string my_map[]={"tok_eof","tok_func", "tok_extern", "tok_identifier","tok_number","tok_LParanteses", "tok_RParanteses",  "tok_LBracket",  "tok_RBracket",  "tok_asign", "data_type",  "tok_for",
"data_arg_array",
"tok_choose",
 "tok_arithmetic_oper",
"tok_in",
"tok_endoperation",
"tok_return",
"tok_delimiter",
"tok_property",
 "tok_logic_oper",
 "tok_main"
 };

static string IdentifierStr;
vector <string> arr;
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
arr.push_back(IdentifierStr);
  if (IdentifierStr == "func")
    return tok_func;
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
  std::string NumStr;
  do {
    NumStr += LastChar;
    LastChar = fin.get();
  } while (isdigit(LastChar) || LastChar == '.');
    arr.push_back(NumStr);
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
arr.push_back(intermediar);
/////////////////////////////////L and R parantheses

if (LastChar == '(') {
  parStack++;
  LastChar=fin.get();
    return tok_LParanteses;
}
if (LastChar == ')') {
  parStack--;
  LastChar=fin.get();
    return tok_RParanteses;
}
if (LastChar == '{') {
  brStack++;
  LastChar=fin.get();
    return tok_LBracket;
}
if (LastChar == '}') {
  brStack--;
  LastChar=fin.get();
    return tok_RBracket;
}
if (LastChar == '|') {
  LastChar=fin.get();
    return tok_choose;
}
if (LastChar == ';') {
  LastChar=fin.get();
    return tok_endoperation;
}
if (LastChar == ',') {
  LastChar=fin.get();
    return tok_delimiter;
}
if (LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/') {
  LastChar=fin.get();
    return tok_arithmetic_oper;
}
if (LastChar == '<' || LastChar == '>') {
  LastChar=fin.get();
    return tok_logic_oper;
}
if ( LastChar == '=') {
  LastChar=fin.get();
    return tok_asign;
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



int main()
{
    vector< int > tokens;
    vector<string> tokenstring;
    int tok;

    while(!fin.eof()){
    tok=gettok();
  //  cout<<tok<<endl;
    tokens.push_back(tok);
     tokenstring.push_back(my_map[-tok-1]);

    }
    for (unsigned i = 0; i < tokenstring.size(); i++) {
	std::cout << tokenstring.at(i) << '\n';
}

for (unsigned i = 0; i < arr.size(); i++) {
	std::cout <<"'" << arr.at(i) << "' ";
}
    return 0;
}
