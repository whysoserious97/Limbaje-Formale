#include <iostream>
#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include<fstream>
#include <assert.h>
#include <memory>


using namespace std;

////instance of...
template<typename Base, typename T>
inline bool instanceof(const T*) {
    return is_base_of<Base, T>::value;
}


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
enum Token {
    tok_eof = -1,

    // commands
    tok_def = -2, tok_extern = -3,

    // primary
    tok_identifier = -4, tok_number = -5,tok_if = -6, tok_then = -7, tok_else = -8,
    tok_for = -9, tok_in = -10,tok_var = -13
};

/*string my_map[]={"tok_eof","tok_def", "tok_extern", "tok_identifier","tok_number","tok_LParanteses", "tok_RParanteses",  "tok_LBracket",  "tok_RBracket",  "data_type",  "tok_for",
                 "data_arg_array",
                 "tok_choose",
                 "tok_arithmetic_oper",
                 "tok_in",
                 "tok_return",
                 "tok_property",
                 "tok_logic_oper",
                 "tok_main"
};*/

static string IdentifierStr;

static double NumVal;
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
        if (IdentifierStr == "def") return tok_def;
        if (IdentifierStr == "extern") return tok_extern;
        if (IdentifierStr == "if") return tok_if;
        if (IdentifierStr == "then") return tok_then;
        if (IdentifierStr == "else") return tok_else;
        if (IdentifierStr == "for") return tok_for;
        if (IdentifierStr == "in") return tok_in;
     //   if (IdentifierStr == "binary") return tok_binary;
        if (IdentifierStr == "var") return tok_var;
        return tok_identifier;
    }
    if (isdigit(LastChar)) {   // Number: [0-9.]+
        string NumStr;
        do {
            NumStr += LastChar;
            LastChar = fin.get();
        } while (isdigit(LastChar) || LastChar == '.');
        tempTok.value=NumStr;
        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    intermediar=LastChar;
    tempTok.value=intermediar;
/////////////////////////////////L and R parantheses

/////////////////////////////////Comments
    if (LastChar == '$') {
        do
            LastChar = fin.get();
        while (LastChar != '\0' && LastChar != '\n' && LastChar != '\r');

        if (LastChar != '\0')
            return gettok();
    }

    if (LastChar == EOF)
        return tok_eof;

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
        virtual ~ExprAST() = default;

    };


/// NumberExprAST - Expression class for numeric literals like "1.0".
    class NumberExprAST : public ExprAST {
    public:
        string Val;

    public:
        NumberExprAST(string Val) : Val(Val) {}
    };

/// VariableExprAST - Expression class for referencing a variable, like "a".
    class VariableExprAST : public ExprAST {
    public:
        std::string Name;

    public:
        VariableExprAST(const std::string &Name) : Name(Name) {}
    };

/// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST {
    public:char Op;
        std::unique_ptr<ExprAST> LHS, RHS;

    public:
        BinaryExprAST(char Op, std::unique_ptr<ExprAST> LHS,
                      std::unique_ptr<ExprAST> RHS)
                : Op(Op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    };

/// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST {
    public:
        std::string Callee;
        std::vector<std::unique_ptr<ExprAST>> Args;

    public:
        CallExprAST(const std::string &Callee,
                    std::vector<std::unique_ptr<ExprAST>> Args)
                : Callee(Callee), Args(std::move(Args)) {}
    };
    /// IfExprAST - Expression class for if/then/else.
    class IfExprAST : public ExprAST {
        ExprAST *Cond, *Then, *Else;
    public:
        IfExprAST(ExprAST *cond, ExprAST *then, ExprAST *_else)
                : Cond(cond), Then(then), Else(_else) {}

    };

/// ForExprAST - Expression class for for/in.
    class ForExprAST : public ExprAST {
        std::string VarName;
        ExprAST *Start, *End, *Step, *Body;
    public:
        ForExprAST(const std::string &varname, ExprAST *start, ExprAST *end,
                   ExprAST *step, ExprAST *body)
                : VarName(varname), Start(start), End(end), Step(step), Body(body) {}

    };
/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;

    public:
        PrototypeAST(const std::string &Name, std::vector<std::string> Args)
                : Name(Name), Args(std::move(Args)) {}

        const std::string &getName() const { return Name; }
    };

/// FunctionAST - This class represents a function definition itself.
    class FunctionAST : public ExprAST {
    public:
        std::unique_ptr<PrototypeAST> Proto;

    public:
        FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                    std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}

        std::unique_ptr<ExprAST> Body;
    };
    /// VarExprAST - Expression class for var/in
    class VarExprAST : public ExprAST {
    public:
        std::vector<std::pair<std::string, ExprAST*> > VarNames;
        ExprAST *Body;
    public:
        VarExprAST(const std::vector<std::pair<std::string, ExprAST*> > &varnames,
                   ExprAST *body)
                : VarNames(varnames), Body(body) {}


    };
} // end anonymous namespace

//===----------------------------------------------------------------------===//
// Parser
//===----------------------------------------------------------------------===//

/// CurTok/getNextToken - Provide a simple token buffer.  CurTok is the current
/// token the parser is looking at.  getNextToken reads another token from the
/// lexer and updates CurTok with its results.
static int CurTok;
static int getNextToken() { return CurTok = objarray[++x].id; }

/// BinopPrecedence - This holds the precedence for each binary operator that is
/// defined.
static std::map<char, int> BinopPrecedence;

/// GetTokPrecedence - Get the precedence of the pending binary operator token.
static int GetTokPrecedence() {
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TokPrec = BinopPrecedence[CurTok];
    if (TokPrec <= 0)
        return -1;
    return TokPrec;
}

/// LogError* - These are little helper functions for error handling.
std::unique_ptr<ExprAST> LogError(const char *Str) {
    fprintf(stderr, "Error: %s\n", Str);
    return nullptr;
}
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str) {
    LogError(Str);
    return nullptr;
}

static std::unique_ptr<ExprAST> ParseExpression();

/// numberexpr ::= number
static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(objarray[x].value);
    getNextToken(); // consume the number
    return std::move(Result);
}

/// parenexpr ::= '(' expression ')'
static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken(); // eat (.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("expected ')'");
    getNextToken(); // eat ).
    return V;
}

/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static std::unique_ptr<ExprAST> ParseIdentifierExpr() {
    std::string IdName = objarray[x].value;

    getNextToken(); // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return std::make_unique<VariableExprAST>(IdName);

    // Call.
    getNextToken(); // eat (
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')') {
        while (true) {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                return nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list");
            getNextToken();
        }
    }

    // Eat the ')'.
    getNextToken();

    return std::make_unique<CallExprAST>(IdName, std::move(Args));
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
static std::unique_ptr<ExprAST> ParsePrimary() {
    switch (CurTok) {
        default:
            return LogError("unknown token when expecting an expression");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

/// binoprhs
///   ::= ('+' primary)*
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec,
                                              std::unique_ptr<ExprAST> LHS) {
    // If this is a binop, find its precedence.
    while (true) {
        int TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current binop,
        // consume it, otherwise we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // Okay, we know this is a binop.
        int BinOp = CurTok;
        getNextToken(); // eat binop

        // Parse the primary expression after the binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS, let
        // the pending operator take RHS as its LHS.
        int NextPrec = GetTokPrecedence();
        if (TokPrec < NextPrec) {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS = std::make_unique<BinaryExprAST>(BinOp, std::move(LHS),
                                              std::move(RHS));
    }
}

/// expression
///   ::= primary binoprhs
///
static std::unique_ptr<ExprAST> ParseExpression() {
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/// prototype
///   ::= id '(' id* ')'
static std::unique_ptr<PrototypeAST> ParsePrototype() {
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");

    std::string FnName = objarray[x].value;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(objarray[x].value);
    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // success.
    getNextToken(); // eat ')'.

    return std::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/// definition ::= 'def' prototype expression
static std::unique_ptr<FunctionAST> ParseDefinition() {
    getNextToken(); // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return std::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    return nullptr;
}
vector<FunctionAST*> exparr;
/// toplevelexpr ::= expression
static FunctionAST* ParseTopLevelExpr() {
    if (auto E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = std::make_unique<PrototypeAST>("__anon_expr",
                                                    std::vector<std::string>());
        auto func=std::make_unique<FunctionAST>(std::move(Proto), std::move(E)).release();

        exparr.push_back(func);
        return func;
    }
    return nullptr;
}

/// external ::= 'extern' prototype
static std::unique_ptr<PrototypeAST> ParseExtern() {
    getNextToken(); // eat extern.
    return ParsePrototype();
}

//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
    auto def = ParseDefinition();
    if (def) {
        auto ptr = def.release();
        exparr.push_back(ptr);
    //    fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern() {
    if (ParseExtern()) {
    //    fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression() {
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
      //  fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop() {
    while (true) {

        switch (CurTok) {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}

void mostrestrictive(ExprAST* q,int level){
    auto e=dynamic_cast<BinaryExprAST*>(q);
    auto var = dynamic_cast<VariableExprAST*>(q);
    auto call = dynamic_cast<CallExprAST*>(q);
    auto number = dynamic_cast<NumberExprAST*>(q);
    auto func = dynamic_cast<FunctionAST *>(q);
    for (int i = 0; i <level;i++){
        cout<<'\t';
    }

    if (e){
        cout<<"Operator:  "<<e->Op<<endl;
        mostrestrictive(e->LHS.get(),level+1);
        mostrestrictive(e->RHS.get(),level+1);
    } else if(var)
    {
        cout<<"Var name:"<<var->Name<<endl;

    } else if(call)
    {
        cout<<"Is calling: "<<call->Callee<<endl;

        for(int i=0;i<call->Args.size();i++){
            for (int j = 0; j <level;j++){
                cout<<'\t';
            }
            cout<<"Parameter:"<<i<<":"<<endl;
            mostrestrictive(call->Args[i].release(),level+1);
        }
    } else if(number){
        cout<<"Is number with value: "<<number->Val<<endl;
    }else if(func){
        mostrestrictive(func,level+1);
    }
    else{
        cout<<"Else"<<endl;
    }
    level++;
}
template<typename Type>
void printfunc(Type a){
    int level=0;
        if (typeid(a)== typeid(FunctionAST*)){
            FunctionAST* b = (FunctionAST*) a;
            auto x = b->Body.release();
            mostrestrictive(x,level);
        }

}
int main()
{

    BinopPrecedence['='] = 5;
    BinopPrecedence['<'] = 10;
    BinopPrecedence['>'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;  // highest.
    BinopPrecedence['/'] = 40;  // highest.

    int tok;
    while(!fin.eof()){
        tok=gettok();
    //    cout<<tok<<'\n';
        //  cout<<tok<<endl;
        tempTok.id= tok;

        objarray.push_back(tempTok);
    }
    tempTok.id=tok_eof;
//    tempTok.name = my_map[-tok_eof-1];
    tempTok.value="EOF";
    objarray.push_back(tempTok);

   /* for (unsigned i = 0; i < objarray.size(); i++) {
        cout <<"id: "<<objarray[i].id<<"\t "<< "\t = \'" +objarray[i].value <<"\'"<< '\n';
    }
    */
    getNextToken();
    MainLoop();
    for(int i=0;i<exparr.size();i++) {
        printfunc(exparr[i]);
    }
    return 0;
}
