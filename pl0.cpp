#include<iostream>
#include<unordered_map>
#include<vector>
#include<algorithm>
#include<typeinfo>

using namespace std;

string TEST_PROGRAM = "var i, s;\n\
begin\n\
    i := 0; s := 0;\n\
    while i < 5 do\n\
    begin\n\
        i := i + 1;\n\
        s := s + i * i\n\
    end\n\
end.\n\
";

bool isDIGIT(char ch)
{
    if(ch >= '0' && ch <= '9')
    {
        return true;
    }
    return false;
}

bool isIDENT_FIRST(char ch)
{
    if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_')
    {
        return true;
    }
    return false;
}

bool isIDENT_REMAIN(char ch)
{
    if(isIDENT_FIRST(ch) || (ch >= '0' && ch <= '9'))
    {
        return true;
    }
    return false;
}

bool isOP(char ch)
{
    string ops = "=#+-*/,.;()";
    if(find(ops.begin(), ops.end(), ch) != ops.end())
    {
        return true;
    }
    return false;
}

int stringToInt(string str)
{
    int ans = 0;
    int n = str.size();
    for(int i = 0; i < n; i++)
    {
        ans = ans * 10 + str[i] - '0';
    }
    return ans;
}

string intToString(int i)
{
    string ans = "";
    while(i)
    {
        ans += i % 10 + '0';
        i /= 10;
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

string KEYWORD_SET[] = 
{
    "const",
    "var",
    "procedure",
    "call",
    "begin",
    "end",
    "if",
    "then",
    "while",
    "do",
    "odd",
};


// class TokenKind
// {
// public:
//     static const int Op = 0;
//     int Num = 1;
//     int Name = 2;
//     int KeyWord = 3;
//     int Eof = 4;
// };

unordered_map<string, int> TokenKindStringToInt
{
    {"Op", 0},
    {"Num", 1},
    {"Name", 2},
    {"KeyWord", 3},
    {"Eof", 4}
};

unordered_map<int, string> TokenKindIntToString
{
    {0, "Op"},
    {1, "Num"},
    {2, "Name"},
    {3, "KeyWord"},
    {4, "Eof"}
};

class Token
{
public:
    int ty;
    int valInt;
    string valString;

    Token(int ty, int valInt, string valString)
    {
        this -> ty = ty;
        this -> valInt = valInt;
        this -> valString = valString;
    }
};

ostream& operator<<(ostream& cout, const Token token)
    {
        if(token.ty == 1)
        {
            cout << "[type: " << TokenKindIntToString[token.ty] << " val: " << token.valInt << "]"; 
        }
        else
        {
            cout << "[type: " << TokenKindIntToString[token.ty] << " val: " << token.valString << "]";
        }
        
    }

class Lexer
{
public:
    int i;
    string s;

    Lexer(string src)
    {
        this -> i = 0;
        this -> s = src;
    }

    bool eof()
    {
        return this -> i >= this -> s.size();
    }

    void _skip_blank()
    {
        while(!this -> eof() && this -> s[this -> i] == ' ')
        {
            this -> i++;
        }
    }

    Token next()
    {
        string val = "";
        this -> _skip_blank();

        if(this -> eof())
        {
            return Token(TokenKindStringToInt["Eof"], 0, "");
        }

        else if(isDIGIT(this -> s[this -> i]))
        {
            while(isDIGIT(this -> s[this -> i]))
            {
                val += this -> s[this -> i];
                this -> i++;
            }
            int num = stringToInt(val);
            return Token(TokenKindStringToInt["Num"], num, "");
        }

        else if(isIDENT_FIRST(this -> s[this -> i]))
        {
            while(isIDENT_REMAIN(this -> s[this -> i]))
            {
                val += this -> s[this -> i];
                this -> i++;
            }
            
            if(find(begin(KEYWORD_SET), end(KEYWORD_SET), val) != end(KEYWORD_SET))
            {
                return Token(TokenKindStringToInt["KeyWord"], 0, val);
            }
            else
            {
                return Token(TokenKindStringToInt["Name"], 0, val);
            }
        }

        else if(isOP(this -> s[this -> i]))
        {
            char ch = this -> s[this -> i];
            this -> i++;
            return Token(TokenKindStringToInt["Op"], 0, string(1, ch));
        }

        else if(this -> s[this -> i] == ':')
        {
            this -> i++;
            
            if(this -> eof() || this -> s[this -> i] != '=')
            {
                throw "'=' expected";
            }

            this -> i++;
            return Token(TokenKindStringToInt["Op"], 0, ":=");
        }

        else if(this -> s[this -> i] == '>' || this -> s[this -> i] == '<')
        {
            char ch = this -> s[this -> i];
            this -> i++;

            if(!this -> eof() & this -> s[this -> i] == '=')
            {
                this -> i++;
                string str = "";
                str.push_back(ch);
                str.push_back('=');
                return Token(TokenKindStringToInt["Op"], 0, str);
            }
        }

        else if(this -> s[this -> i] == '\n')
        {
            this -> i++;
        }

        else
        {
            throw "invalid character";
        }
    }
};





















class Factor;
class Term;
class Expression;
class Assign;
class Begin;
class OddCondition;
class StdCondition;
class Condition;
class If;
class While;
class Statement;
class Procedure;
class Block;
class Program;

class Factor
{
public:
    string valString;
    int valInt;
    Expression* valExpr;
    Factor(){};
    Factor(const Factor& factor)
    {
        this -> valString = factor.valString;
        this -> valInt = factor.valInt;
        this -> valExpr = factor.valExpr;
    }
    Factor(string valString, int valInt, Expression* ValExpr)
    {
        this -> valString = valString;
        this -> valInt = valInt;
        this -> valExpr = valExpr;

        // this -> valExpr = new Expression;
        // this -> valExpr -> mod = valExpr->mod;
        // this -> valExpr -> lhs = new Term;
    }
};

class Term
{
public:
    Factor* lhs;
    vector<pair<string, Factor*>> rhs;
    Term(){};
    Term(const Term& term)
    {
        this -> lhs = term.lhs;
        this -> rhs = term.rhs;
    }
    Term(Factor* lhs, vector<pair<string, Factor*>> rhs)
    {
        this -> lhs = lhs;
        this -> rhs = rhs;
    }
};

class Expression
{
public:
    string mod;
    Term* lhs;
    vector<pair<string, Term*>> rhs;
    Expression(){};
    Expression(const Expression& expr)
    {
        this -> mod = expr.mod;
        this -> lhs = expr.lhs;
        this -> rhs = expr.rhs;
    }
    Expression(string mod, Term* lhs, vector<pair<string, Term*>> rhs)
    {
        this -> mod = mod;
        this -> lhs = lhs;
        this -> rhs = rhs;
    }
};

class Const
{
public:
    string name;
    int value;

    Const();
    Const(const Const& _const)
    {
        this -> name = _const.name;
        this -> value = _const.value;
    }
    Const(string name, int value)
    {
        this -> name = name;
        this -> value = value;
    }
};

class Assign
{
public:
    string name;
    Expression* expr;

    Assign(){};
    Assign(const Assign& assign)
    {
        this -> name = assign.name;
        this -> expr = assign.expr;
    }
    Assign(string name, Expression* expr)
    {
        this -> name = name;
        this -> expr = expr;
    }
};

class Call
{
public:
    string name;
    Call(){};
    Call(const Call& call)
    {
        this -> name = call.name;
    }
    Call(string name)
    {
        this -> name = name;
    }
};

class Begin
{
public:
    vector<Statement*> body;

    Begin(){};
    Begin(const Begin& begin)
    {
        this -> body = begin.body;
    }
    Begin(vector<Statement*> body)
    {
        this -> body = body;
    }
};

class OddCondition
{
public:
    Expression* expr;
    OddCondition(){};
    OddCondition(const OddCondition& odd_cond)
    {
        this -> expr = odd_cond.expr;
    }
    OddCondition(Expression* expr)
    {
        this -> expr = expr;
    }
};

class StdCondition
{
public:
    string op;
    Expression* lhs;
    Expression* rhs;
    StdCondition(){};
    StdCondition(const StdCondition& std_cond)
    {
        this -> op = std_cond.op;
        this -> lhs = std_cond.lhs;
        this -> rhs = std_cond.rhs;
    }
    StdCondition(string op, Expression* lhs, Expression* rhs)
    {
        this -> op = op;
        this -> lhs = lhs;
        this -> rhs = rhs;
    }
};

class Condition
{
public:
    OddCondition* oddCond;
    StdCondition* stdCond;
    Condition(){};
    Condition(const Condition& cond)
    {
        this -> oddCond = cond.oddCond;
        this -> stdCond = cond.stdCond;
    }
    Condition(OddCondition* odd_ptr, StdCondition* std_ptr)
    {
        this -> oddCond = odd_ptr;
        this -> stdCond = std_ptr;
    }
};

class If
{
public:
    Condition* cond;
    Statement* then;
    If(){};
    If(const If& _if)
    {
        this -> cond = _if.cond;
        this -> then = _if.then;
    }
    If(Condition* cond, Statement* then)
    {
        this -> cond = cond;
        this -> then = then;
    }
};

class While
{
public:
    Condition* cond;
    Statement* then;
    While(){};
    While(const While& _while)
    {
        this -> cond = _while.cond;
        this -> then = _while.then;
    }
    While(Condition* cond, Statement* then)
    {
        this -> cond = cond;
        this -> then = then;
    }
};

class Statement
{
public:
    Assign* stmtA;
    Begin* stmtB;
    Call* stmtC;
    If* stmtI;
    While* stmtW;

    Statement(){};
    Statement(const Statement& state)
    {
        this -> stmtA = state.stmtA;
        this -> stmtB = state.stmtB;
        this -> stmtC = state.stmtC;
        this -> stmtI = state.stmtI;
        this -> stmtW = state.stmtW;
    }
    Statement(Assign* stmtA, Begin* stmtB, Call* stmtC, If* stmtI, While* stmtW)
    {
        this -> stmtA = stmtA;
        this -> stmtB = stmtB;
        this -> stmtC = stmtC;
        this -> stmtI = stmtI;
        this -> stmtW = stmtW;
    }
};

class Procedure
{
public:
    string name;
    Block* body;

    Procedure(){};
    Procedure(const Procedure& pro)
    {
        this -> name = pro.name;
        this -> body = pro.body;
    }
    Procedure(string name, Block* body)
    {
        this -> name = name;
        this -> body = body;
    }
};

class Block
{
public:
    vector<Const*> consts;
    vector<string> vars;
    vector<Procedure*> procs;
    Statement* stmt;

    Block(vector<Const*> consts, vector<string> vars, vector<Procedure*> procs, Statement* stmt)
    {
        this -> consts = consts;
        this -> vars = vars;
        this -> procs = procs;
        this -> stmt = stmt;
    }

    Block(){};
    Block(const Block& block)
    {
        this -> consts = block.consts;
        this -> vars = block.vars;
        this -> procs = block.procs;
        this -> stmt = block.stmt;
    }
};

class Program
{
public:
    Block* block;
    Program(Block* block)
    {
        this -> block = block;
    }
};

class Parser
{
public:
    Lexer* lx;

    Parser(Lexer* lx)
    {
        this -> lx = lx;
    }

    bool check(int ty, string valString, int valInt)
    {
        int p = this -> lx -> i;
        Token tk = this -> lx -> next();

        if(tk.ty == ty && tk.valInt == valInt && tk.valString == valString)
        {
            return true;
        }

        this -> lx -> i = p;
        return false;
    }

    void expect(int ty, string valString, int valInt)
    {
        Token tk = this -> lx -> next();
        int tty = tk.ty;
        string tvalString = tk.valString;
        int tvalInt = tk.valInt;

        if(tty != ty)
        {
            throw ty + " expected, got " + tty;
        }

        if(tty == TokenKindStringToInt["Num"] && valInt != tvalInt)
        {
            throw intToString(valInt) + " expected, got " + intToString(tvalInt);
        }

        if(tty != TokenKindStringToInt["Num"] && valString != tvalString)
        {
            throw valString + " expected, got " + tvalString;
        }
    }

    Program program();
    Block block();
    vector<Const*> _const();
    vector<string> var();
    Procedure procedure();
    Statement statement();
    Condition condition();
    OddCondition odd_condition();
    StdCondition std_condition();
    Expression expression();
    Term term();
    Factor factor();
};

Program Parser::program()
{
    Block block = this -> block();
    this -> expect(TokenKindStringToInt["Op"], ".", 0);
    return Program(&block);
}

Block Parser::block()
{
    vector<string> vars;
    vector<Procedure*> procs;
    vector<Const*> consts;

    if(this -> check(TokenKindStringToInt["KeyWord"], "const", 0))
    {
        consts = this -> _const();
    }

    if(this -> check(TokenKindStringToInt["KeyWord"], "var", 0))
    {
        vars = this -> var();
    }

    while (this -> check(TokenKindStringToInt["KeyWord"], "procedure", 0))
    {
        auto proc = this -> procedure();
        procs.push_back(&proc);
    }
    
    Statement stmt = this -> statement();
    return Block(consts, vars, procs, &stmt); 
}

vector<Const*> Parser::_const()
{
    vector<Const*> ans;
    while(1)
    {
        Token name = this -> lx -> next();
        int ty = name.ty;

        if(ty != TokenKindStringToInt["Name"])
        {
            throw "name expected";
        }

        this -> expect(TokenKindStringToInt["Op"], "=", 0);
        Token num = this -> lx -> next();

        if(num.ty != TokenKindStringToInt["Num"])
        {
            throw "number expected";
        }
        else
        {
            Const* con = new Const(name.valString, num.valInt);
            ans.push_back(con);
        }

        if(this -> check(TokenKindStringToInt["Op"], ";", 0))
        {
            return ans;
        }
        else
        {
            this -> expect(TokenKindStringToInt["Op"], ",", 0);
        }
    }
}

vector<string> Parser::var()
{
    vector<string> ans;
    while (1)
    {
        Token name = this -> lx -> next();
        int ty = name.ty;

        if(ty != TokenKindStringToInt["Name"])
        {
            throw "name expected";
        }
        else
        {
            ans.push_back(name.valString);
        }

        if(this -> check(TokenKindStringToInt["Op"], ";", 0))
        {
            return ans;
        }
        else
        {
            this -> expect(TokenKindStringToInt["Op"], ",", 0);
        }
    }
}

Procedure Parser::procedure()
{
    Token name = this -> lx -> next();
    int ty = name.ty;

    if(ty != TokenKindStringToInt["Name"])
    {
        throw "name expected";
    }
    this -> expect(TokenKindStringToInt["Op"], ";", 0);

    Block block = this -> block();
    this -> expect(TokenKindStringToInt["Op"], ";", 0);

    return Procedure(name.valString, &block);
}

Statement Parser::statement()
{
    Assign* ass = new Assign;
    Begin* beg = new Begin;
    Call* cal = new Call;
    If* _if = new If;
    While* whi = new While;
    if(this -> check(TokenKindStringToInt["KeyWord"], "call", 0))
    {
        Token ident = this -> lx -> next();
        if(ident.ty != TokenKindStringToInt["Name"])
        {
            throw "name expected";
        }
        else
        {
            cal->name = ident.valString;
            return Statement(ass, beg, cal, _if, whi);
        }
    }

    else if(this -> check(TokenKindStringToInt["KeyWord"], "begin", 0))
    {
        vector<Statement*> body;

        while (1)
        {
            Statement stmt = this -> statement();
            body.push_back(&stmt);

            if(this -> check(TokenKindStringToInt["KeyWord"], "end", 0))
            {
                break;
            }
            else
            {
                this -> expect(TokenKindStringToInt["Op"], ";", 0);
            }
        }

        beg -> body = body; 
        return Statement(ass, beg, cal, _if, whi);
    }

    else if(this -> check(TokenKindStringToInt["KeyWord"], "if", 0))
    {
        Condition cond = this -> condition();
        this -> expect(TokenKindStringToInt["KeyWord"], "then", 0);
        _if -> cond = &cond;
        auto stmt = this -> statement();
        _if -> then = &stmt;
        return Statement(ass, beg, cal, _if, whi);
    }

    else if(this -> check(TokenKindStringToInt["KeyWord"], "while", 0))
    {
        Condition cond = this -> condition();
        this -> expect(TokenKindStringToInt["KeyWord"], "then", 0);
        whi -> cond = &cond;
        auto stmt = this -> statement();
        whi -> then = &stmt;
        return Statement(ass, beg, cal, _if, whi);
    }

    else
    {
        Token tk = this -> lx -> next();
        int ty = tk.ty;

        if(ty != TokenKindStringToInt["Name"])
        {
            throw "name expected";
        }

        this -> expect(TokenKindStringToInt["Op"], ":=", 0);
        ass -> name = tk.valString;
        auto expr = this -> expression();
        ass -> expr = &expr;
        return Statement(ass, beg, cal, _if, whi);
    }
}

Condition Parser::condition()
{
    if(this -> check(TokenKindStringToInt["KeyWord"], "odd", 0))
    {
        auto odd = this -> odd_condition();
        return Condition(&odd, nullptr);
    }
    else
    {
        auto std = this -> std_condition();
        return Condition(nullptr, &std);
    }
}

OddCondition Parser::odd_condition()
{
    auto expr = this -> expression();
    return OddCondition(&expr);
}

StdCondition Parser::std_condition()
{
    Expression lhs = this -> expression();
    Token cmp = this -> lx -> next();

    if(cmp.ty != TokenKindStringToInt["Op"])
    {
        throw "operator expected";
    }

    vector<string> op_list = {"=", "#", "<", ">", "<=", ">="};
    if(find(op_list.begin(), op_list.end(), cmp.valString) == op_list.end())
    {
        throw "condition operator expected";
    }

    Expression rhs = this -> expression();
    return StdCondition(cmp.valString, &lhs, &rhs);
}

Expression Parser::expression()
{
    string mod = "";
    if(this -> check(TokenKindStringToInt["Op"], "+", 0))
    {
        mod = "+";
    }
    else if(this -> check(TokenKindStringToInt["Op"], "-", 0))
    {
        mod = "-";
    }

    vector<pair<string, Term*>> rhs;
    Term lhs = this -> term();

    while(1)
    {
        if(this -> check(TokenKindStringToInt["Op"], "+", 0))
        {
            auto term = this -> term();
            rhs.push_back(pair<string, Term*>{"+", &term});
        }
        else if(this -> check(TokenKindStringToInt["Op"], "-", 0))
        {
            auto term = this -> term();
            rhs.push_back(pair<string, Term*>{"-", &term});
        }
        else
        {
            break;
        }
    }
    return Expression(mod, &lhs, rhs);
}

Term Parser::term()
{
    vector<pair<string, Factor*>> rhs;
    Factor lhs = this -> factor();

    while(1)
    {
        if(this -> check(TokenKindStringToInt["Op"], "*", 0))
        {
            auto factor = this -> factor();
            rhs.push_back(pair<string, Factor*>{"*", &factor});
        }
        else if(this -> check(TokenKindStringToInt["Op"], "/", 0))
        {
            auto factor = this -> factor();
            rhs.push_back(pair<string, Factor*>{"/", &factor});
        }
        else
        {
            break;
        }
    }
    return Term(&lhs, rhs);
}

Factor Parser::factor()
{
    Token tk = this -> lx -> next();
    int ty = tk.ty;
    int valInt = tk.valInt;
    string valString = tk.valString;

    if(ty == TokenKindStringToInt["Num"])
    {
        auto expr = new Expression;
        return Factor("", valInt, expr);
    }
    if(ty == TokenKindStringToInt["Name"])
    {
        auto expr = new Expression;
        return Factor(valString, 0, expr);
    }

    if(ty != TokenKindStringToInt["Op"] || valString != "(")
    {
        throw "'(' expected";
    }

    Expression expr = this -> expression();
    this -> expect(TokenKindStringToInt["Op"], ")", 0);
    return Factor("", 0, &expr);
}


int main()
{
    cout << TEST_PROGRAM << endl;
    
    // Lexer lx(TEST_PROGRAM);
    // Token tk = lx.next();
    // while (tk.ty != TokenKindStringToInt["Eof"])
    // {
    //     cout << tk << endl;
    //     tk = lx.next();
    // }

    Lexer* lx = new Lexer(TEST_PROGRAM);
    Parser ps = Parser(lx);
    // cout << ps.program();

    return 0;
}