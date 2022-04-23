#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
// for future assignments leave it as it is
class parser
{
    lexer _lexer;

public:
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

    // EXPRESSION
    bool EXPR(); // wrapper E()
    bool E();
    bool T();
    bool E_();
    bool T_();
    bool F();
    // ASSIGNMENT
    bool ASSIGN();
    // FOR
    bool FOR();

    bool CODE_BODY();

    bool CODE();

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
    bool COMMENT_BODY();
    bool STRING();
    bool ASCII();
    bool LETTER();
    bool SPECIAL();

    // OPERATOR
    bool ARTH_OP();
    bool RELATIONAL_OP();

    // PRINT
    bool PRINT();
    bool PRINT_BODY();

    // RETURN
    bool RETURN();
    bool RETURN_BODY();

    // ASSIGNMENT COND
    bool ASSIGN_COND();

    // CONDITIONAL STATEMENT
    bool COND();
};
#endif
