#include "parser.h"

void parser::syntax_error()
{
    cout << "SYNTAX ERROR\n";
    exit(1);
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
    if (t.tokenType != expected_type)
        syntax_error();
    return t;
}
parser::parser(const char filename[])
{
    _lexer = lexer(filename);
}
void parser::readAndPrintAllInput() // read and print allinputs (provided)
{
    token t;
    t = _lexer.getNextToken();
    while (t.tokenType != TokenType::END_OF_FILE)
    {
        t.Print();
        t = _lexer.getNextToken();
    }
}
void parser::resetPointer()
{
    _lexer.resetPointer();
}
// this function is for sample purposes only
/*
bool parser::statements()
{
    //statements-- > COLON LPAREN start RPAREN
    if (_lexer.peek(1).tokenType == TokenType::COLON)
    {
        expect(TokenType::COLON);
        if (_lexer.peek(1).tokenType == TokenType::LPAREN)
        {
            expect(TokenType::LPAREN);
            start();
            if (_lexer.peek(1).tokenType == TokenType::RPAREN)
            {
                expect(TokenType::RPAREN);
                return true;
            }
        }
    }
    return false;
}*/

////////////// EXPR ///////////////

/**
Expression -> T E`
E` -> +TE` | -TE` | ^
T -> FT`
T` -> *FT` | /FT` | % FT | ^
F -> id | num
 */

bool isEqual(const string &a, const string &b)
{
    return a.compare(b) == 0;
}

bool isEqual(const string &a, vector<string> &b)
{
    for (auto i : b)
    {
        if (i.compare(a) == 0)
            return true;
    }
    return false;
}

bool parser::EXPR()
{
    return E();
}

bool parser::E()
{
    return T() && E_();
}
// E` -> +TE` | -TE' | ^
bool parser::E_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (isEqual(_c, {"+", "-"}))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return T() && E_();
    }
}
// T -> FT`
bool parser::T()
{
    return F() && T_();
}
// T' -> *FT'| /FT' | % FT | ^
bool parser::T_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (isEqual(_c, {"*", "/", "%"}))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return T() && E_();
    }
}
// F -> id | num
bool parser::F()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        expect(TokenType::IDENTIFIER);
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUMERIC_LITERAL)
    {
        expect(TokenType::IDENTIFIER);
    }
}
