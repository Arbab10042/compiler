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
    else
    {
        cout << "Accepted token: ";
        t.Print();
        cout << "\n";
        return t;
    }
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

void parser::debug(const string &where, const string &messages, token t)
{
    if (DEBUG)
    {
        cout << "DEBUG: " << where << ": " << messages << ": ";
        t.Print();
    }
}

bool isEqual(const string &a, const string &b)
{
    return a.compare(b) == 0;
}

////////////// START //////////////

// Start -> Function Start | Comment Start
bool parser::START()
{
    // Start -> Function Start | Comment Start
    if (FUNC())
    {
        // START();
        bool a = START();
        if (_lexer.getCurrentToken().tokenType == TokenType::END_OF_FILE)
        {
            expect(TokenType::END_OF_FILE);
            return true;
        }
        return a;
    }
    else if (COMMENT())
    {
        bool a = START();
        if (_lexer.getCurrentToken().tokenType == TokenType::END_OF_FILE)
        {
            expect(TokenType::END_OF_FILE);
            return true;
        }
        return a;
    }
    debug(__func__, "Failed", _lexer.peek(1));
    return false;
}

////////////// EXPR ///////////////

/**
Expression -> T E` .
E` -> +TE` | -TE` | .
T -> F T` .
T` -> * F T` | / F T` | % F T | .
F -> id | num .
 */
bool parser::EXPR()
{
    return E();
}
bool parser::E()
{
    return T() && E_();
}
// E` -> +TE` | -TE` | .
bool parser::E_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (isEqual(_c, "+") || isEqual(_c, "-"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return T() && E_();
    }
    else if (_c[0] == '+' || _c[0] == '-')
    {
        return T() && E_();
    }
    return true;
}
// T -> F T` .
bool parser::T()
{
    return F() && T_();
}
// T` -> * F T` | / F T` | % F T | .
bool parser::T_()
{
    const string _c = _lexer.peek(1).lexeme;
    if (isEqual(_c, "*") || isEqual(_c, "/") || isEqual(_c, "%"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return F() && T_();
    }
    return true;
}
// F -> id | num .
bool parser::F()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        expect(TokenType::IDENTIFIER);
        return true;
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUMERIC_LITERAL)
    {
        expect(TokenType::NUMERIC_LITERAL);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));
    return false;
}

////////////////// PRINT /////////////////////
/*
Print -> print PrintBody ; .
PrintBody -> Expression | String .
*/

bool parser::PRINT()
{
    if (_lexer.peek(1).tokenType == TokenType::PRINT)
    {
        expect(TokenType::PRINT);
        if (PRINT_BODY())
        {
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                return true;
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

bool parser::PRINT_BODY()
{
    return EXPR() || STRING();
}

/////////////// COMMENT ////////////////

//  COMMENT -> # StringBody EOF .
bool parser::COMMENT()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMENT)
    {
        const string _c = _lexer.peek(1).lexeme;

        // actual CFG but won't run for our lexer :)
        // if (StringBody())
        // {
        //     if (_lexer.peek(1).tokenType == TokenType::END_OF_FILE)
        //     {
        //         return true;
        //     }
        // }

        if (_c[0] == '#')
        {
            expect(TokenType::COMMENT);
            return true;
        }
    }
    debug(__func__, "COMMENT FAILED", _lexer.peek(1));
    return false;
}

/////////////// STRING ////////////////

/*
String -> " StringBody " .
StringBody -> char StringBody | num StringBody | .
*/
bool parser::STRING()
{
    const string _c = _lexer.peek(1).lexeme;

    if (_c[0] == '"' && _c[_c.size() - 1] == '"')
    {
        expect(TokenType::STRING);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

/////////////////// ASSIGNMENT //////////////////////

/**
Assignment -> id Assignment2 .
Assignment2 -> , Assignment | <- Assignment3 | Type ; .
Assignment3 -> Expression Assignment4 .
Assignment4 -> , Assignment | Type ; .
 */

// Assignment -> id Assignment2 .
bool parser::ASSIGN()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        expect(TokenType::IDENTIFIER);
        if (ASSIGN_2())
        {
            return true;
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// Assignment2 -> , Assignment | <- Assignment3 | Type ; .
bool parser::ASSIGN_2()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        return ASSIGN();
    }
    else if (_lexer.peek(1).tokenType == TokenType::ASSIGNMENT_OPERATOR)
    {
        expect(TokenType::ASSIGNMENT_OPERATOR);
        if (ASSIGN_3())
        {
            return true;
        }
    }
    else if (TYPE())
    {
        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
        {
            expect(TokenType::SEMICOLON);
            return true;
        }
    }
    return false;
}

// Assignment3 -> Expression Assignment4 .
bool parser::ASSIGN_3()
{
    if (EXPR())
    {
        return ASSIGN_4();
    }
    return false;
}

// Assignment4 -> , Assignment | Type ; | ; .
bool parser::ASSIGN_4()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        return ASSIGN();
    }
    else if (TYPE())
    {
        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
        {
            expect(TokenType::SEMICOLON);
            return true;
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
    {
        expect(TokenType::SEMICOLON);
        return true;
    }
    return false;
}

/////////////// TYPE ///////////////////

// Type -> char | int .
bool parser::TYPE()
{
    if (_lexer.peek(1).tokenType == TokenType::CHAR)
    {
        expect(TokenType::CHAR);
        return true;
    }

    else if (_lexer.peek(1).tokenType == TokenType::INT)
    {
        expect(TokenType::INT);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

/////////////////// RETURN //////////////////////

/*
Return -> return ReturnBody ; .
ReturnBody -> Expression | String | .
*/

// Return -> return ReturnBody ; .
bool parser::RETURN()
{
    if (_lexer.peek(1).tokenType == TokenType::RETURN)
    {
        expect(TokenType::RETURN);
        if (RETURN_BODY())
        {
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                return true;
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// ReturnBody -> Expression | String | .
bool parser::RETURN_BODY()
{
    if (EXPR())
        return true;
    else if (STRING())
        return true;
    return true;
}

//////////////// FUNCTION //////////////////////

/*
Function -> func Type id : CodeBody .
*/

// Function -> func Type id : CodeBody .
bool parser::FUNC()
{
    if (_lexer.peek(1).tokenType == TokenType::FUNC)
    {
        expect(TokenType::FUNC);
        if (TYPE())
        {
            if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
            {
                expect(TokenType::IDENTIFIER);
                if (_lexer.peek(1).tokenType == TokenType::COLON)
                {
                    expect(TokenType::COLON);
                    if (CODE())
                    {
                        if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
                        {
                            expect(TokenType::SEMICOLON);
                            return true;
                        }
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

////////////////// IN ////////////////////

// In -> in id ;
bool parser::IN()
{
    if (_lexer.peek(1).tokenType == TokenType::IN)
    {
        expect(TokenType::IN);
        if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
        {
            expect(TokenType::IDENTIFIER);
            if (_lexer.peek(1).tokenType == TokenType::SEMICOLON)
            {
                expect(TokenType::SEMICOLON);
                return true;
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

///////////////// CODE //////////////////////
// Code -> begin CodeBody | Comment Code .
// CodeBody -> If CodeBody CodeBodyEnd | For CodeBody CodeBodyEnd | Print CodeBody CodeBodyEnd | Comment CodeBody CodeBodyEnd | Assignment CodeBody CodeBodyEnd | In CodeBody CodeBodyEnd | Return CodeBody CodeBodyEnd .
// CodeBodyEnd -> end .

// Code -> begin CodeBody .
bool parser::CODE()
{
    if (_lexer.peek(1).tokenType == TokenType::BEGIN)
    {
        expect(TokenType::BEGIN);
        if (CODE_BODY())
        {

            return true;
        }
    }
    else if (COMMENT())
    {
        if (CODE())
        {
            return true;
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// CodeBody -> If CodeBody CodeBodyEnd | For CodeBody CodeBodyEnd | Print CodeBody CodeBodyEnd | Comment CodeBody CodeBodyEnd | Assignment CodeBody CodeBodyEnd | In CodeBody CodeBodyEnd | Return CodeBody CodeBodyEnd .
bool parser::CODE_BODY()
{
    if (IF())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (FOR())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (PRINT())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (COMMENT())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (ASSIGN())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (IN())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    else if (RETURN())
    {
        bool a = CODE_BODY();
        bool b = CODE_BODY_END();
        return a && b;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// CodeBodyEnd -> end .
bool parser::CODE_BODY_END()
{
    if (_lexer.peek(1).tokenType == TokenType::END)
    {
        expect(TokenType::END);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

///////////////// IF ///////////////////
/*
IF -> if CondionalStatement : CodeBody IF_ .
IF_ -> elif ConditionalStatement : CodeBody IF_ | else : CodeBody | .
*/

// IF -> if CondionalStatement : CodeBody IF_ .
bool parser::IF()
{
    if (_lexer.peek(1).tokenType == TokenType::IF)
    {
        expect(TokenType::IF);
        if (CONDITIONAL_STATEMENT())
        {
            if (_lexer.peek(1).tokenType == TokenType::COLON)
            {
                expect(TokenType::COLON);
                CODE();
                {
                    if (IF_())
                    {
                        return true;
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// IF_ -> elif ConditionalStatement : CodeBody IF_ | else : CodeBody | .
bool parser::IF_()
{
    if (_lexer.peek(1).tokenType == TokenType::ELIF)
    {
        expect(TokenType::ELIF);
        if (CONDITIONAL_STATEMENT())
        {
            if (_lexer.peek(1).tokenType == TokenType::COLON)
            {
                expect(TokenType::COLON);
                CODE();
                {
                    if (IF_())
                    {
                        return true;
                    }
                }
            }
        }
    }
    else if (_lexer.peek(1).tokenType == TokenType::ELSE)
    {
        expect(TokenType::ELSE);
        if (_lexer.peek(1).tokenType == TokenType::COLON)
        {
            expect(TokenType::COLON);
            if (CODE())
            {
                return true;
            }
        }
    }
    return true;
}

//////////////////// CONDITIONAL STATEMENT ////////////////////////

// ConditionalStatement -> Expression RelationalOP Expression .
bool parser::CONDITIONAL_STATEMENT()
{
    if (EXPR())
    {
        if (RELATIONAL_OPERATOR())
        {
            if (EXPR())
            {
                return true;
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

/////////////////////// FOR ////////////////////////////////

// For -> for Assignment, ConditionalStatement, Assignment : Code .
bool parser::FOR()
{
    if (_lexer.peek(1).tokenType == TokenType::FOR)
    {
        expect(TokenType::FOR);
        if (FOR_ASSIGN())
        {
            if (_lexer.peek(1).tokenType == TokenType::COMMA)
            {
                expect(TokenType::COMMA);
                if (CONDITIONAL_STATEMENT())
                {
                    if (_lexer.peek(1).tokenType == TokenType::COMMA)
                    {
                        expect(TokenType::COMMA);
                        if (FOR_ASSIGN())
                        {
                            if (_lexer.peek(1).tokenType == TokenType::COLON)
                            {
                                expect(TokenType::COLON);
                                if (CODE())
                                {
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

// FOR_ASSIGN -> id <- Expression .
bool parser::FOR_ASSIGN()
{
    if (_lexer.peek(1).tokenType == TokenType::IDENTIFIER)
    {
        expect(TokenType::IDENTIFIER);
        if (_lexer.peek(1).tokenType == TokenType::ASSIGNMENT_OPERATOR)
        {
            expect(TokenType::ASSIGNMENT_OPERATOR);
            if (EXPR())
            {
                return true;
            }
        }
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

//////////////////////// ARTHIMATIC OPERATORS //////////////////////////////

// ArthimaticOP -> + | - | * | / | % .
bool parser::ARITHEMATIC_OPERATOR()
{
    const string _c = _lexer.peek(1).lexeme;
    if (isEqual(_c, "+") || isEqual(_c, "-") || isEqual(_c, "*") || isEqual(_c, "/") || isEqual(_c, "%"))
    {
        expect(TokenType::ARITHEMATIC_OPERATOR);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}

///////////////////////// REALTIONAL OPERATORS //////////////////////////////

// RelationalOP -> = | ~ = | < RelationalOP' | > RelationalOP' .
// RelationalOP' -> = | .

// RelationalOP -> = | ~ = | < RelationalOP' | > RelationalOP' .
bool parser::RELATIONAL_OPERATOR()
{
    const string _c = _lexer.peek(1).lexeme;
    // if (isEqual(_c, "=") || isEqual(_c, "~=") || isEqual(_c, "<") || isEqual(_c, ">") || isEqual(_c, ">=") || isEqual(_c, "<="))
    if (isEqual(_c, "=") || isEqual(_c, "~=") || isEqual(_c, "NE") || isEqual(_c, "LT") || isEqual(_c, "GT") || isEqual(_c, "GE") || isEqual(_c, "LE"))
    {
        expect(TokenType::RO);
        return true;
    }
    debug(__func__, "Failed", _lexer.peek(1));

    return false;
}