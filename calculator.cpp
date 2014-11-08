//  Freddy Yang



#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <map>

using namespace std;

// This is the token type, and contains all possible tokens in our language.
typedef enum {
    T_PLUS,
    T_MINUS,
    T_MULTIPLY,
    T_POWER,
    T_DIVIDE,
    T_OPENPAREN,
    T_CLOSEPAREN,
    T_OPENBRACKET,
    T_CLOSEBRACKET,
    T_EQUALS,
    T_SEMICOLON,
    T_M,
    T_PRINT,
    T_NUMBER,
    T_EOF
} token;

// This function will convert a token to a string, for display.
std::string tokenToString(token toConvert) {
    switch (toConvert) {
        case T_PLUS:
            return std::string("+");
        case T_MINUS:
            return std::string("-");
        case T_MULTIPLY:
            return std::string("*");
        case T_POWER:
            return std::string("**");
        case T_DIVIDE:
            return std::string("/");
        case T_OPENPAREN:
            return std::string("(");
        case T_CLOSEPAREN:
            return std::string(")");
        case T_OPENBRACKET:
            return std::string("[");
        case T_CLOSEBRACKET:
            return std::string("]");
        case T_EQUALS:
            return std::string("=");
        case T_SEMICOLON:
            return std::string(";");
        case T_M:
            return std::string("M");
        case T_PRINT:
            return std::string("print");
        case T_NUMBER:
            return std::string("number");
        case T_EOF:
            return std::string("EOF");
    }
}

// Throw this error when the parser expects a given token from the scanner
// and the next token the scanner finds does not match.
void mismatchError(int line, token expected, token found) {
    std::cerr << "Parse error: expected " << tokenToString(expected) << " found " << tokenToString(found) << " at line " << line << std::endl;
    exit(1);
}

// Throw this error when the parser encounters a token that is not valid
// at the beginning of a specific grammar rule.
void parseError(int line, token found) {
    std::cerr << "Parse error: found invalid token " << tokenToString(found) << " at line " << line << std::endl;
    exit(1);
}

// Throw this error when an invalid character is found in the input, or
// if a bad character is found as part of a print token.
void scanError(int line, char badCharacter) {
    std::cerr << "Scan error: found invalid character " << badCharacter << " at line " << line << std::endl;
    exit(1);
}

// Throw this error when the index for memory is out of bounds (less
// than 0 or greater than 99). ONLY THROW IF evaluate flag is true.
void indexError(int line, int index) {
    std::cerr << "Semantic error: index " << index << " out of bounds at line " << line << std::endl;
    exit(1);
}

// Throw this error when a division by zero occurs. ONLY THROW IF evaluate flag is true.
void divideError(int line) {
    std::cerr << "Semantic error: division by zero at line " << line << std::endl;
    exit(1);
}

class Scanner {
    // You are allowed to private fields to the scanner, and this may be
    // necessary to complete your implementation. However, this is not
    // required as part of the project specification.
    
public:
    Scanner();
    token nextToken(); 
    void eatToken(token);
    int lineNumber();
    int getNumberValue();

    char nextChar();
    void loopUnget(int num);
    void loopCin (int num);
private:
    int lineNum;
    bool shiftRight;
    token tokenNext;
    char cur;
    string number;
};

//int Scanner::lineNum = 1;
Scanner::Scanner()
{
    lineNum = 1;
    shiftRight= true;
    number = "";
}


char Scanner::nextChar()
{
    char c = cin.get();
    //char c = std::cin.peek();
    return c;
}

void Scanner::loopUnget (int num)
{
    for (int i = 0; i < num; i++)
        cin.unget();
}

void Scanner::loopCin (int num)
{
    for (int i = 0; i < num; i++)
        getchar();
}

// **********************   Scanner   ************************* 
/*

token Scanner::nextToken() {
    // This is a placeholder token, you will need to replace this code
    // with code to return the correct next token.


    // if(shiftRight == false) 
    //     return tokenNext;
    // char c = getchar();
    //cout << c << endl;

    char c = std::cin.peek();
    switch (c) {
        case '+':
        {
            tokenNext = T_PLUS;
            break;
        }
        case '-':
        {
            tokenNext = T_MINUS;
            break;
        }
        case '*':
        {
            nextChar();
            char peek = cin.peek();
            if (peek == '*')
            {
                tokenNext = T_POWER;
                break;
            }
            else
            {
                tokenNext = T_MULTIPLY;
                cin.unget();
                break;
            }
            // tokenNext = T_MULTIPLY;
            // nextChar();

            // if (nextChar() == '*')
            // {
            //     tokenNext = T_POWER;
            //     loopUnget(2);
            //     break;
            // }
            // else
            // {
            //     loopUnget(2);
            //     break;
            // }
            break;
        }
        case '/':
        {
            tokenNext = T_DIVIDE;
            break;
        }
        case '(':
        {
            tokenNext = T_OPENPAREN;
            break;
        }
        case ')':
        {
            tokenNext = T_CLOSEPAREN;
            break;
        }
        case '[':
        {
            tokenNext = T_OPENBRACKET;
            break;
        }
        case ']':
        {
            tokenNext = T_CLOSEBRACKET;
            break;
        }
        case '=':
        {
            tokenNext = T_EQUALS;
            break;
        }
        case ';':
        {
            tokenNext = T_SEMICOLON;
            break;
        }
        case 'm':
        {
            tokenNext = T_M;
            break;
        }
        case 'p':
        {
            if (nextChar() == 'p')
                if (nextChar() == 'r')
                    if (nextChar() == 'i')
                        if (nextChar() == 'n')
                        {
                            // if (cin.peek() == 't')
                            // {
                            //     tokenNext = T_PRINT;
                            // }
                            char peek = cin.peek();   
                            if (peek == 't')
                            {
                                tokenNext = T_PRINT;
                                loopUnget(4);
                            }
                            else
                                scanError(lineNum, c);
                        }
                //         else
                //             scanError(lineNum, c);
                //     else 
                //         scanError(lineNum, c);
                // else
                //     scanError(lineNum, c);
            break;
        }

        //Case: number
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            tokenNext = T_NUMBER;
            break;
            
            ////tokenNext = T_NUMBER;
            ////c = nextChar();
            ////while (isdigit(c))
            ////{
            ////    tokenNext = T_NUMBER;
            ////    c = nextChar()
            ////    c,f
            ////}
                        // int i = 0;
            // do{
            //     char next = cin.peek();
            //     if (next >='0' && next <= '9')
            //         nextChar();

            //     else{
            //         cin.unget();
            //         break;
            //     }
            // } while (true);


        // all white-space cases
        // Case: space
        case ' ': 
        {         
            nextChar();
            return nextToken(); 
        }
        // Case: horizontal tab
        case '\t':
        {
            nextChar();
            return nextToken(); 
        }
        // Case: newline
        case '\n':
        {
            lineNum++;
            nextChar();
            return nextToken(); 
        }
        // Case: vertical tab
        case '\v':
        {
            nextChar();
            return nextToken(); 
        }
        // Case: feed
        case '\f':
        {
            nextChar();
            return nextToken(); 
        }
        // Carriage Return
        case '\r':
        {
            nextChar();
            return nextToken();   
        }
        case EOF:
        {
            tokenNext = T_EOF;
            break;
        }
        default:
        {
            scanError(lineNum, c);
            break;
        }
    };


    return tokenNext;
}


void Scanner::eatToken(token toConsume) {
    // This function consumes the next token.   
    // WRITEME
    if( toConsume != nextToken())
         mismatchError(lineNumber(),toConsume,nextToken());
    //shiftRight = true;
    else
    {   
        switch(toConsume)
        {
            case T_POWER:
                loopCin(2);
                break;
            case T_PRINT:
                loopCin(5);
                break;
            case T_NUMBER:
            {
                char next = nextChar();
                char peek = cin.peek();
                while (peek >= '0' && peek <= '9')
                {
                    peek = cin.peek();
                    nextChar();
                }
                break;
            }
            default:
                nextChar();
                break;
        }

    }
}
*/

// WHY DOES MY PEEK() & UNGET() NOT WORKINNNNNNNNNNNNNNN. OMGGGGG


// **********************   Scanner (2nd Attempt)   ************************* 
// SECOND ATTEMP: Use a boolean inside the eatToken/nextToken to start/stop shifting (THIS WAY PUTS THE BUFFER NEAR THE END OF EACH TOKEN, BUT GIVE OUT SAME RESULT)
token Scanner::nextToken() {
    // This is a placeholder token, you will need to replace this code
    // with code to return the correct next token.
    
    //std:: cout << shiftRight <<std::endl;
    if(shiftRight == false){ 
        return tokenNext;
    //char c = std::cin.tokenNext();
    }
    else{
    number = "";
    char c = getchar();
    //char c = cin.peek();
    //std::cout << c << " ";

    switch (c) {
        case '+':
            tokenNext = T_PLUS;
            break;
        case '-':
            tokenNext = T_MINUS;
            break;
        case '*':
            if (nextChar() == '*')
            {
                tokenNext = T_POWER;
            }
            else
                tokenNext = T_MULTIPLY;
            break;
        case '/':
            tokenNext = T_DIVIDE;
            break;
        case '(':
            tokenNext = T_OPENPAREN;
            break;
        case ')':
            tokenNext = T_CLOSEPAREN;
            break;
        case '[':
            tokenNext = T_OPENBRACKET;
            break;
        case ']':
            tokenNext = T_CLOSEBRACKET;
            break;
        case '=':
            tokenNext = T_EQUALS;
            break;
        case ';':
            tokenNext = T_SEMICOLON;
            break;
        case 'm':
            tokenNext = T_M;
            break;
        case 'p':
        {   
            char next;
            next = nextChar();
            if (next == 'r')
            {
                char next;
                next = nextChar();
                if (next == 'i')
                {
                    char next;
                    next = nextChar();
                    if (next == 'n')
                    {
                        char next;
                        next = nextChar();
                        if (next == 't')
                        {
                           char next;
                           next = nextChar();
                           tokenNext = T_PRINT;  
                        }
                        else
                        {
                            scanError(lineNum, next);
                        }
                    }
                    else
                    {
                       scanError(lineNum, next);
                    }
                }
                else 
                {
                    scanError(lineNum, next);
                }
            }
            else
                scanError(lineNum, next);
            break;
        }
        // Case: number
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            tokenNext = T_NUMBER;
            number+=c;
            while (isdigit(cin.peek()))
            {
                number+=cin.get();;
            }
            break;
        }
        // all white-space cases
        // Case: space
        case ' ':          
            return nextToken();
            break;
        // Case: horizontal tab
        case '\t':
            return nextToken();
        // Case: newline
        case '\n':
        {
            lineNum++;
            return nextToken();
        // Case: vertical tab
        }
        case '\v':
             return nextToken();
        // Case: feed
        case '\f':
             return nextToken();
        // Carriage Return
        case '\r':
             return nextToken();     
        case EOF:
            tokenNext = T_EOF;
            break;
        default:
            scanError(lineNum, c);
            break;
    };

    shiftRight = false;
    //std::cout << tokenToString(tokenNext) << " ";
    return tokenNext;
    }
}

void Scanner::eatToken(token toConsume) {
    // This function consumes the next token.   
    if( toConsume != nextToken() )
         mismatchError(lineNumber(),toConsume,nextToken());
    // GIVE PERMISSION TO SHIFTRIGHT, ELSE ALWAYS STAY IN THE SAME TOKEN
    shiftRight = true;
    nextToken();
}

int Scanner::lineNumber() {
    // This is also a placeholder, you will need to replace this code
    // with code to return the current line number (to be used for errors).
    
    return lineNum;       //lineNUm is a can be found in nextToken()
}

int Scanner::getNumberValue() {
    // This is also a placeholder, you will need to replace this code
    // with code to return the last number value that was scanned. This
    // will be used when evaluating expressions.
    
    // WRITEME
    int returnNum = atoi(number.c_str());
    //return returnNum;
    return returnNum;
}

class Parser {
    // You are allowed to private fields to the parser, and this may be
    // necessary to complete your implementation. However, this is not
    // required as part of the project specification.
    
private:
    Scanner scanner;
    
    // This flag indicates whether we should perform evaluation and throw
    // out-of-bounds and divide-by-zero errors. ONLY evaluate and throw these
    // errors if this flag is set to TRUE.
    bool evaluate;
    
    // You will need to add more methods for handling nonterminal symbols here.
    void Start();
    // WRITEME
    void Statements();
    void Term();
    void Statement();
    void Expression();
    void Factor();
    void Exp_Tail();
    void Factor_Tail();
    void Power();
    void Power_Tail();
    void Final();
    int num_parser;

public:
    std::map <int, int> mDict;
    void parse();
    Parser(bool evaluate) : evaluate(evaluate) {}
};

// **********************   Parser   ************************* 
//  Following grammar is derived from: http://theory.stanford.edu/~amitp/yapps/yapps-doc/node2.html
//  Start      -> StatementS
//  StatementS -> Statement Term
//  Term       -> ; Statement Term | Epsolon
//  Statement  -> "m" [Expression] = Expression | print Expression
//  Expression -> Factor Exp_Tail
//  Exp_Tail   -> "+" Factor Exp_Tail | "-" Factor Exp_Tail | EPSOLON
//  Factor     -> power Factor_Tail
//  Factor_Tail-> "*"" power Factor_Tail |  "/"" power Factor_Tail | Epsolon
//  power      -> final Power_Tail
//  power_Tail -> "**" final power_Tail
//  final       -> Num | "(" Expression ")" | m [ Expression ]

void Parser::parse()
{
    Start();
}

//  Start -> StatementS
void Parser::Start()
{
    Statements();
}

//  StatementS -> Statement Term
void Parser::Statements()
{
    Statement();
    if (scanner.nextToken() == T_SEMICOLON)
        Term();

}

//  Term -> ; Statement Term | Epsolon
void Parser::Term()
{
    //cout<< tokenToString(scanner.nextToken()) <<endl;
    switch (scanner.nextToken()) {
        case T_SEMICOLON:       //;
        {
            scanner.eatToken(T_SEMICOLON);
            Statement();
            if (scanner.nextToken() == T_SEMICOLON)
                Term();
            //Term();
            break;
        }
        case T_EOF:
            break;
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
    }
}

// Statement  -> "m" "[" Expression "]" "=" Expression | print Expression
void Parser::Statement()
{
    switch(scanner.nextToken())
    {
        case T_M:
        {

            scanner.eatToken(T_M);
            scanner.eatToken(T_OPENBRACKET);


            switch(scanner.nextToken())
            {
                case T_M:
                case T_NUMBER:
                case T_OPENPAREN:
                    Expression();
                case T_EOF:
                    break;
            }

            scanner.eatToken(T_CLOSEBRACKET);
            scanner.eatToken(T_EQUALS);
            

            switch(scanner.nextToken())
            {
                case T_M:
                case T_NUMBER:
                case T_OPENPAREN:
                    Expression();
                case T_EOF:
                    break;
            }


            break;
        }


        case T_PRINT:
        {
            scanner.eatToken(T_PRINT);
            switch(scanner.nextToken())
            {
                case T_M:
                case T_NUMBER:
                case T_OPENPAREN:
                    Expression();
                case T_EOF:
                    break;
            }
            break;
        }
        case T_EOF:
            break;
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
  }
}

// Expression -> Factor Exp_Tail
void Parser::Expression()
{

    Factor();
    switch (scanner.nextToken())
    {
        case T_PLUS:            // +
        {
            Exp_Tail();
            break;
        }
        case T_MINUS:           // -
        {
            Exp_Tail();
            break;
        }     
    }
}

//Exp_Tail -> "+" Factor Exp_Tail | "-" Factor Exp_Tail | EPSOLON
void Parser::Exp_Tail()
{
    switch (scanner.nextToken())
    {
        case T_PLUS:            // +
        {
            scanner.eatToken(T_PLUS);
            Factor();
            if (scanner.nextToken() == T_PLUS || scanner.nextToken() == T_MINUS)
                Exp_Tail();
            break;
        }
        case T_MINUS:           // -
        {
            scanner.eatToken(T_MINUS);
            Factor();
            if (scanner.nextToken() == T_PLUS || scanner.nextToken() == T_MINUS)
                Exp_Tail();
            break;
        }
        case T_EOF:
            break;  
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
    }

}

//  Factor -> power Factor_Tail
void Parser:: Factor()
{
    Power();
    switch (scanner.nextToken()) {
        case T_MULTIPLY:        // *
        {
            Factor_Tail();
            break;
        }
        case T_DIVIDE:          // /
        {
            Factor_Tail();
            break;
        }
    }
}


//  Factor_Tail-> "*" power Factor_Tail |  "/"" power Factor_Tail | Epsolon
void Parser::Factor_Tail()
{
    switch (scanner.nextToken()) {
        case T_MULTIPLY:        // *
        {
            scanner.eatToken(T_MULTIPLY);
            Power();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE)
                Factor_Tail();
            break;
        }
        case T_DIVIDE:          // /
        {
            scanner.eatToken(T_DIVIDE);
            Power();
            if (scanner.nextToken() == T_MULTIPLY || scanner.nextToken() == T_DIVIDE)
                Factor_Tail();
            break;
        }
        case T_EOF:
            break;  
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
    }

}


//  power -> final Power_Tail
void Parser::Power()
{   
    Final();
    if (scanner.nextToken() == T_POWER)
        Power_Tail();
}


//  power_Tail -> "**" final power_Tail | Epsilon
void Parser::Power_Tail()
{
    switch (scanner.nextToken()) {
        case T_POWER:           // **
        {
            scanner.eatToken(T_POWER);
            Final();
            if (scanner.nextToken() == T_POWER)
                Power_Tail();
            break;
        }
        case T_EOF:
            break;  
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
    }
}

//  final -> Num | "(" Expression ")" | m [ Expression ]
void Parser:: Final()
{
    switch (scanner.nextToken())
    {
        case T_OPENPAREN:       // (
        {
            scanner.eatToken(T_OPENPAREN);
            
            switch(scanner.nextToken())
            {
                case T_M:
                case T_NUMBER:
                case T_OPENPAREN:
                    Expression();
                case T_EOF:
                    break;
            }



            scanner.eatToken(T_CLOSEPAREN);
            break;
        }
        case T_M:               // m
        {
            scanner.eatToken(T_M);
            scanner.eatToken(T_OPENBRACKET);
            

            switch(scanner.nextToken())
            {
                case T_M:
                case T_NUMBER:
                case T_OPENPAREN:
                    Expression();
                case T_EOF:
                    break;
            }



            scanner.eatToken(T_CLOSEBRACKET);
            break;
        }
        case T_NUMBER:          // num
        {
            scanner.eatToken(T_NUMBER);
            num_parser = scanner.getNumberValue(); 
            break;
        }
        case T_EOF:
            break;  
        default:
        {
            parseError(scanner.lineNumber(), scanner.nextToken());
            break;
        }
    }
}




// **********************   Main   ************************* 
int main(int argc, char* argv[]) {
    if (argc == 2 && (strcmp(argv[1], "-s") == 0)) {
        Scanner scanner;
        while (scanner.nextToken() != T_EOF) {
//            std::cout << (scanner.nextToken()) << " ";
            std::cout << tokenToString(scanner.nextToken()) << " ";
            scanner.eatToken(scanner.nextToken());
        }

        std::cout<<std::endl;
   }
    else if (argc == 2 && (strcmp(argv[1], "-e") == 0)) {
        Parser parser(true);
        parser.parse();
    } else {
        Parser parser(false);
        parser.parse();
    }
    return 0;
}
