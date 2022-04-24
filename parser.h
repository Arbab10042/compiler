#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
// for future assignments leave it as it is
class parser
{
    lexer _lexer;

public:
    bool DEBUG = true;

    void debug(const string &where, const string &messages, token t);

    void syntax_error();
    token expect(TokenType expected_type);
    parser(const char filename[]);
    void readAndPrintAllInput();
    void resetPointer();
    /*Terminal functions goes here use peek and expect*/
    /*use TokenType:: for token names for example
        expect(TokenType::ASSIGN);   //example function call
      */
    // all your parser function goes here

    bool START();

    // EXPRESSION
    bool EXPR(); // wrapper E()
    bool E();
    bool T();
    bool E_();
    bool T_();
    bool F();
    // ASSIGNMENT
    bool ASSIGN();
    bool ASSIGN_();
    // FOR
    bool FOR();

    bool CODE();
    bool CODE_BODY();
    bool CODE_BODY_END();

    bool IF();
    bool IF_();

    // VARIABLE
    bool VAR(); // wrapper V()
    bool V();
    bool V_();

    bool TYPE();

    // FUNCTION
    bool FUNC();

    // COMMENT
    bool COMMENT();
    bool STRING();
    bool STRING_BODY();

    // OPERATOR
    bool ARITHEMATIC_OPERATOR();
    bool RELATIONAL_OPERATOR();
    bool RELATIONAL_OPERATOR_();

    // PRINT
    bool PRINT();
    bool PRINT_BODY();

    // RETURN
    bool RETURN();
    bool RETURN_BODY();

    // CONDITIONAL STATEMENT
    bool CONDITIONAL_STATEMENT();
};
#endif
