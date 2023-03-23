#include<iostream>
#include<unordered_map>
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

string KEYWORD_SET[] = {
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

unordered_map<string, int> TokenKindStringToInt{
    {"Op", 0},
    {"Num", 1},
    {"Name", 2},
    {"KeyWord", 3},
    {"Eof", 4}
};

unordered_map<int, string> TokenKindIntToString{
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






int main()
{
    cout << TEST_PROGRAM << endl;
    
    Lexer lx(TEST_PROGRAM);
    Token tk = lx.next();
    while (tk.ty != TokenKindStringToInt["Eof"])
    {
        cout << tk << endl;
        tk = lx.next();
    }
    

    return 0;
}