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


int x=-1;   //current index of parsing


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
    tok_identifier = -4, tok_number = -5,

    // control
    tok_if = -6, tok_then = -7, tok_else = -8,
    tok_for = -9, tok_in = -10,

    // operators
    tok_binary = -11, tok_unary = -12,

    // var definition
    tok_var = -13
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
        virtual ~ExprAST() {}

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
/// UnaryExprAST - Expression class for a unary operator.
    class UnaryExprAST : public ExprAST {
    public:
        char Opcode;
        ExprAST *Operand;
    public:
        UnaryExprAST(char opcode, ExprAST *operand)
                : Opcode(opcode), Operand(operand) {}

    };
/// BinaryExprAST - Expression class for a binary operator.
    class BinaryExprAST : public ExprAST {
    public:char Op;
        ExprAST *LHS, *RHS;

    public:
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs)
                : Op(op), LHS(lhs), RHS(rhs) {}
    };

/// CallExprAST - Expression class for function calls.
    class CallExprAST : public ExprAST {
    public:
        std::string Callee;
        std::vector<ExprAST*> Args;

    public:
        CallExprAST(const std::string &callee, std::vector<ExprAST*> &args)
                : Callee(callee), Args(args) {}
    };
    /// IfExprAST - Expression class for if/then/else.
    class IfExprAST : public ExprAST {
    public:
        ExprAST *Cond, *Then, *Else;
    public:
        IfExprAST(ExprAST *cond, ExprAST *then, ExprAST *_else)
                : Cond(cond), Then(then), Else(_else) {}

    };

/// ForExprAST - Expression class for for/in.
    class ForExprAST : public ExprAST {
    public:
        std::string VarName;
        ExprAST *Start, *End, *Step, *Body;
    public:
        ForExprAST(const std::string &varname, ExprAST *start, ExprAST *end,
                   ExprAST *step, ExprAST *body)
                : VarName(varname), Start(start), End(end), Step(step), Body(body) {}

    };
    /// VarExprAST - Expression class for var/in
    class VarExprAST : public ExprAST {
    public:
    public:
        std::vector<std::pair<std::string, ExprAST*> > VarNames;
     //   ExprAST *Body;
    public:
        VarExprAST(const std::vector<std::pair<std::string, ExprAST*> > &varnames
                   )
                : VarNames(varnames) {}


    };
/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its argument names as well as if it is an operator.
    class PrototypeAST {
        std::string Name;
        std::vector<std::string> Args;
        bool isOperator;
        unsigned Precedence;  // Precedence if a binary op.
    public:
        PrototypeAST(const std::string &name, const std::vector<std::string> &args,
                     bool isoperator = false, unsigned prec = 0)
                : Name(name), Args(args), isOperator(isoperator), Precedence(prec) {}

        bool isUnaryOp() const { return isOperator && Args.size() == 1; }
        bool isBinaryOp() const { return isOperator && Args.size() == 2; }

        char getOperatorName() const {
            assert(isUnaryOp() || isBinaryOp());
            return Name[Name.size()-1];
        }

        unsigned getBinaryPrecedence() const { return Precedence; }



        //void CreateArgumentAllocas(Function *F);
    };

/// FunctionAST - This class represents a function definition itself.
    class FunctionAST : public ExprAST {
    public:
        PrototypeAST *Proto;
        ExprAST *Body;

    public:
        FunctionAST(PrototypeAST *proto, ExprAST *body)
                : Proto(proto), Body(body) {}

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
/// Error* - These are little helper functions for error handling.
ExprAST *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
PrototypeAST *ErrorP(const char *Str) { Error(Str); return 0; }
FunctionAST *ErrorF(const char *Str) { Error(Str); return 0; }

static ExprAST *ParseExpression();
/// identifierexpr
///   ::= identifier
///   ::= identifier '(' expression* ')'
static ExprAST *ParseIdentifierExpr() {
    std::string IdName = objarray[x].value;

    getNextToken();  // eat identifier.

    if (CurTok != '(') // Simple variable ref.
        return new VariableExprAST(IdName);

    // Call.
    getNextToken();  // eat (
    std::vector<ExprAST*> Args;
    if (CurTok != ')') {
        while (true) {
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
    ExprAST *Result = new NumberExprAST(objarray[x].value);
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

/// ifexpr ::= 'if' expression 'then' expression 'else' expression
static ExprAST *ParseIfExpr() {
    getNextToken();  // eat the if.

    // condition.
    ExprAST *Cond = ParseExpression();
    if (!Cond) return 0;

    if (CurTok != tok_then)
        return Error("expected then");
    getNextToken();  // eat the then

    ExprAST *Then = ParseExpression();
    if (Then == 0) return 0;

    if (CurTok != tok_else)
        return Error("expected else");

    getNextToken();

    ExprAST *Else = ParseExpression();
    if (!Else) return 0;

    return new IfExprAST(Cond, Then, Else);
}

/// forexpr ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
static ExprAST *ParseForExpr() {
    getNextToken();  // eat the for.

    if (CurTok != tok_identifier)
        return Error("expected identifier after for");

    std::string IdName = IdentifierStr;
    getNextToken();  // eat identifier.

    if (CurTok != '=')
        return Error("expected '=' after for");
    getNextToken();  // eat '='.


    ExprAST *Start = ParseExpression();
    if (Start == 0) return 0;
    if (CurTok != ',')
        return Error("expected ',' after for start value");
    getNextToken();

    ExprAST *End = ParseExpression();
    if (End == 0) return 0;

    // The step value is optional.
    ExprAST *Step = 0;
    if (CurTok == ',') {
        getNextToken();
        Step = ParseExpression();
        if (Step == 0) return 0;
    }

    if (CurTok != tok_in)
        return Error("expected 'in' after for");
    getNextToken();  // eat 'in'.

    ExprAST *Body = ParseExpression();
    if (Body == 0) return 0;

    return new ForExprAST(IdName, Start, End, Step, Body);
}

/// varexpr ::= 'var' identifier ('=' expression)?
//                    (',' identifier ('=' expression)?)* 'in' expression
static ExprAST *ParseVarExpr() {
    getNextToken();  // eat the var.

    std::vector<std::pair<std::string, ExprAST*> > VarNames;

    // At least one variable name is required.
    if (CurTok != tok_identifier)
        return Error("expected identifier after var");

    while (1) {
        std::string Name = objarray[x].value;
        getNextToken();  // eat identifier.

        // Read the optional initializer.
        ExprAST *Init = 0;
        if (CurTok == '=') {
            getNextToken(); // eat the '='.

            Init = ParseExpression();
            if (Init == 0) return 0;
        }

        VarNames.push_back(std::make_pair(Name, Init));

        // End of var list, exit loop.
        if (CurTok != ',') break;
        getNextToken(); // eat the ','.

        if (CurTok != tok_identifier)
            return Error("expected identifier list after var");
    }

   /* // At this point, we have to have 'in'.
    if (CurTok != tok_in)
        return Error("expected 'in' keyword after 'var'");
    getNextToken();  // eat 'in'.

    ExprAST *Body = ParseExpression();
    if (Body == 0) return 0;*/

    return new VarExprAST(VarNames); //return new VarExprAST(VarNames, Body);
}

/// primary
///   ::= identifierexpr
///   ::= numberexpr
///   ::= parenexpr
///   ::= ifexpr
///   ::= forexpr
///   ::= varexpr
static ExprAST *ParsePrimary() {
    switch (CurTok) {
        default: return Error("unknown token when expecting an expression");
        case tok_identifier: return ParseIdentifierExpr();
        case tok_number:     return ParseNumberExpr();
        case '(':            return ParseParenExpr();
        case tok_if:         return ParseIfExpr();
        case tok_for:        return ParseForExpr();
        case tok_var:        return ParseVarExpr();
    }
}

/// unary
///   ::= primary
///   ::= '!' unary
static ExprAST *ParseUnary() {
    // If the current token is not an operator, it must be a primary expr.
    if (!isascii(CurTok) || CurTok == '(' || CurTok == ',')
        return ParsePrimary();

    // If this is a unary operator, read it.
    int Opc = CurTok;
    getNextToken();
    if (ExprAST *Operand = ParseUnary())
        return new UnaryExprAST(Opc, Operand);
    return 0;
}

/// binoprhs
///   ::= ('+' unary)*
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

        // Parse the unary expression after the binary operator.
        ExprAST *RHS = ParseUnary();
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
///   ::= unary binoprhs
///
static ExprAST *ParseExpression() {
    ExprAST *LHS = ParseUnary();
    if (!LHS) return 0;

    return ParseBinOpRHS(0, LHS);
}

/// prototype
///   ::= id '(' id* ')'
static PrototypeAST *ParsePrototype() {
    std::string FnName;

    unsigned Kind = 0; // 0 = identifier, 1 = unary, 2 = binary.
    unsigned BinaryPrecedence = 30;

    switch (CurTok) {
        default:
            return ErrorP("Expected function name in prototype");
        case tok_identifier:
            FnName = IdentifierStr;
            Kind = 0;
            getNextToken();
            break;
        case tok_unary:
            getNextToken();
            if (!isascii(CurTok))
                return ErrorP("Expected unary operator");
            FnName = "unary";
            FnName += (char)CurTok;
            Kind = 1;
            getNextToken();
            break;
        case tok_binary:
            getNextToken();
            if (!isascii(CurTok))
                return ErrorP("Expected binary operator");
            FnName = "binary";
            FnName += (char)CurTok;
            Kind = 2;
            getNextToken();

            // Read the precedence if present.
            if (CurTok == tok_number) {
                if (NumVal < 1 || NumVal > 100)
                    return ErrorP("Invalid precedecnce: must be 1..100");
                BinaryPrecedence = (unsigned)NumVal;
                getNextToken();
            }
            break;
    }

    if (CurTok != '(')
        return ErrorP("Expected '(' in prototype");

    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);
    if (CurTok != ')')
        return ErrorP("Expected ')' in prototype");

    // success.
    getNextToken();  // eat ')'.

    // Verify right number of names for operator.
    if (Kind && ArgNames.size() != Kind)
        return ErrorP("Invalid number of operands for operator");

    return new PrototypeAST(FnName, ArgNames, Kind != 0, BinaryPrecedence);
}

/// definition ::= 'def' prototype expression
static FunctionAST *ParseDefinition() {
    getNextToken();  // eat def.
    PrototypeAST *Proto = ParsePrototype();
    if (Proto == 0) return 0;

    if (ExprAST *E =
            ParseExpression())
        return new FunctionAST(Proto, E);
    return 0;
}
vector<ExprAST*> exparr;
/// toplevelexpr ::= expression
static FunctionAST *ParseTopLevelExpr() {
    if (ExprAST *E = ParseExpression()) {
        // Make an anonymous proto.
        auto Proto = new PrototypeAST("", std::vector<std::string>());
        auto func =new FunctionAST(Proto, E);
        exparr.push_back(func);
        return func;
    }
    return 0;
}

/// external ::= 'extern' prototype
static PrototypeAST *ParseExtern() {
    getNextToken();  // eat extern.
    return ParsePrototype();
}
/*static FunctionAST* ParseTopLevelExpr() {
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
}*/


//===----------------------------------------------------------------------===//
// Top-Level parsing
//===----------------------------------------------------------------------===//

static void HandleDefinition() {
    auto def = ParseDefinition();
    if (def) {
      //  exparr.push_back(def);
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
    auto unary = dynamic_cast<UnaryExprAST *>(q);
    auto var2 = dynamic_cast<VarExprAST*>(q);
    auto if_fun = dynamic_cast<IfExprAST*>(q);
    auto for_fun = dynamic_cast<ForExprAST*>(q);
    for (int i = 0; i <level;i++){
        cout<<'\t';
    }

    if (e){
        cout<<"Operator:  "<<e->Op<<endl;
        mostrestrictive(e->LHS,level+1);
        mostrestrictive(e->RHS,level+1);
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
            mostrestrictive(call->Args[i],level+1);
        }
    } else if(number){
        cout<<"Is number with value: "<<number->Val<<endl;
    }else if(func){
        mostrestrictive(func->Body,level);
    } else if(unary){
        cout<<unary->Opcode<<endl;
        mostrestrictive(unary->Operand,level);
    }else if(var2){
        for(int j=0;j<var2->VarNames.size();j++){
            for (int j = 0; j <level;j++){
                cout<<'\t';
            }
            cout<<"Declared:"<<var2->VarNames[j].first<<endl;
            mostrestrictive(var2->VarNames[j].second,level);
        }

    }
    else if (if_fun){
        cout<<"Condition:"<<endl;
        mostrestrictive(if_fun->Cond,level);
        cout<<"Then:"<<endl;
        mostrestrictive(if_fun->Then,level);
        cout<<"Else:"<<endl;
        mostrestrictive(if_fun->Else,level);
    }
    else if (for_fun){
        cout<<"Body:"<<endl;
        mostrestrictive(for_fun->Body,level);
        cout<<"Start:"<<endl;
        mostrestrictive(for_fun->Start,level);
        cout<<"End:"<<endl;
        mostrestrictive(for_fun->End,level);
        cout<<"Step:"<<endl;
        mostrestrictive(for_fun->Step,level);
    }
    else{
        cout<<"Value is NULL"<<endl;
    }
    level++;
}
//template<typename Type>
void printfunc(ExprAST *a){
    int level=0;
            mostrestrictive(a,level);


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
