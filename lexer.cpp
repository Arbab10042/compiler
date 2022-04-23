#include "lexer.h"

using namespace std;
// for printing tokens names, Same as the enum defined in lexer.h
string reserved[] = {
    "END_OF_FILE",
    "ERROR",
    "IDENTIFIER",
    "FUNC",
    "INT",
    "CHAR",
    "CALL",
    "IF",
    "ELIF",
    "ELSE",
    "FOR",
    "PRINT",
    "PRINTLN",
    "RETURN",
    "IN",
    "RO",
    "NE",
    "ASSIGNMENT_OPERATOR",
    "ARITHEMATIC_OPERATOR",
    "NUMERIC_LITERAL",
    "CHARACTER_LITERAL",
    "COMMA",
    "SEMICOLON",
    "COLON",
    "BEGIN",
    "END",
    "COMMENT",
    "STRING"};
token::token()
{
    this->lexeme = "";
    this->tokenType = TokenType::ERROR;
}
token::token(string lexeme, TokenType tokenType)
{
    this->lexeme = lexeme;
    this->tokenType = tokenType;
}
void token::Print()
{
    cout << "{" << lexeme << " , "
         << reserved[(int)tokenType] << "}\n";
}
int lexer::getCurrentPointer()
{
    return index;
}
void lexer::setCurrentPointer(int pos)
{
    if (pos >= 0 && pos < tokens.size())
        index = pos;
    else
        index = 0;
}
void lexer::Tokenize() // function that tokenizes your input stream
{
    vector<char>::iterator it = stream.begin();
    int state = 0;
    // your implementation goes here
    // push_back EOF token at end to identify End of File
    string letterStream = "";
    for (; it != stream.end(); it++)
    {
        switch (state)
        {
        case 0: // Starting state
            if (*it == 'r')
                state = 44;
            else if (*it == 'p')
                state = 36;
            else if (*it == 'e')
                state = 25;
            else if (*it == 'c')
                state = 20;
            else if (*it == 'f')
                state = 15;
            else if (*it == 'i')
                state = 10;
            else if (*it == 'b')
                state = 65;
            else if (*it == '<')
                state = 1;
            else if (*it == '>')
                state = 2;
            else if (*it == '~')
                state = 7;
            else if (*it == '#')
                state = 84;
            else if (*it >= '0' && *it <= '9')
            {
                if (*it != '\t' && *it != '\n')
                    state = 86;
            }
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it == '"')
                state = 62;
            else if (*it == ',')
                state = 101;
            else if (*it == ':')
                state = 102;
            else if (*it == ';')
                state = 103;
            if (*it != '\n' && *it != '\t' && *it != ' ')
            {
                letterStream.push_back(*it);
            }
            break;

        case 1:
            if (*it == '=')
                state = 4;
            else if (*it == '-')
                state = 16;
            else
                state = 5;
            letterStream.push_back(*it);
            break;

        case 2:
            if (*it == '=')
                state = 3;
            else
                state = 6;
            letterStream.push_back(*it);
            break;

        case 3: // Final State
            state = 0;
            tokens.push_back(token("GE", TokenType::RO));
            letterStream = "";
            break;

        case 4: // Final State
            state = 0;
            tokens.push_back(token("LE", TokenType::RO));
            letterStream = "";
            break;

        case 5: // Final State
            state = 0;
            --it;
            --it;
            tokens.push_back(token("LT", TokenType::RO));
            letterStream = "";
            break;

        case 6: // Final State
            state = 0;
            --it;
            tokens.push_back(token("GT", TokenType::RO));
            letterStream = "";
            break;

        case 7:
            if (*it == '=')
                state = 8;
            else
                state = 150;
            letterStream.push_back(*it);
            break;

        case 8: // Final State
            state = 0;
            tokens.push_back(token("NE", TokenType::NE));
            letterStream = "";
            break;

        case 10:
            if (*it == 'f')
                state = 11;
            else if (*it == 'n')
                state = 12;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
            {
                state = 89;
            }
            if (*it == ' ')
                letterStream.push_back(*it);
            break;

        case 11:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 96;
            letterStream.push_back(*it);
            break;

        case 12:
            if (*it == 't')
                state = 13;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 14;
            letterStream.push_back(*it);
            break;

        case 13: // FUNC
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 98;
            letterStream.push_back(*it);
            break;

        case 14: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::IN));
            letterStream = "";
            break;

        case 15:
            if (*it == 'u')
                state = 17;
            else if (*it == 'o')
                state = 34;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 16: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::ASSIGNMENT_OPERATOR));
            letterStream = "";
            break;

        case 17:
            if (*it == 'n')
                state = 18;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 18:
            if (*it == 'c')
                state = 19;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 19: // FUNC
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 100;
            letterStream.push_back(*it);
            break;

        case 20:
            if (*it == 'a')
                state = 31;
            else if (*it == 'h')
                state = 21;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 21:
            if (*it == 'a')
                state = 22;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 22:
            if (*it == 'r')
                state = 23;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 23: // CHAR
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 93;
            letterStream.push_back(*it);
            break;

        case 25:
            if (*it == 'n')
                state = 78;
            else if (*it == 'l')
                state = 26;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 26:
            if (*it == 'i')
                state = 28;
            else if (*it == 's')
                state = 27;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 27:
            if (*it == 'e')
                state = 30;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 28:
            if (*it == 'f')
                state = 29;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 29:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 61;
            letterStream.push_back(*it);
            break;

        case 30:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 90;
            letterStream.push_back(*it);
            break;

        case 31:
            if (*it == 'l')
                state = 32;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 32:
            if (*it == 'l')
                state = 33;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 33:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 92;
            letterStream.push_back(*it);
            break;

        case 34:
            if (*it == 'r')
                state = 35;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 35:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 99;
            letterStream.push_back(*it);
            break;

        case 36:
            if (*it == 'r')
                state = 37;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 37:
            if (*it == 'i')
                state = 38;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 38:
            if (*it == 'n')
                state = 39;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 39:
            if (*it == 't')
                state = 40;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 40:
            if (*it == 'l')
                state = 41;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 43; // PRINT
            letterStream.push_back(*it);
            break;

        case 41:
            if (*it == 'n')
                state = 49;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 42: // RETURN
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 56;
            letterStream.push_back(*it);
            break;

        case 43: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::PRINT));
            letterStream = "";
            break;

        case 44:
            if (*it == 'e')
                state = 45;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 45:
            if (*it == 't')
                state = 46;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 46:
            if (*it == 'u')
                state = 47;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;
        case 47:
            if (*it == 'r')
                state = 48;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;
        case 48:
            if (*it == 'n')
                state = 42;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 49: // PRINTLN
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 91;
            letterStream.push_back(*it);
            break;

        case 51:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
            {
                state = 51;
                letterStream.push_back(*it);
            }
            else if (*it >= '0' && *it <= '9')
            {
                letterStream.push_back(*it);
                state = 52;
            }
            else
                state = 89;
            break;

        case 52: // For identifiers having digits at end
            if (*it >= '0' && *it <= '9')
            {
                letterStream.push_back(*it);
                state = 52;
            }
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 150;
            else
                state = 89;
            break;

        case 54:
            if (*it == 'n')
                state = 76;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 55:
            if (*it == 'g')
                state = 68;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 56:
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::RETURN));
            break;

        case 61: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::ELIF));
            letterStream = "";
            break;

        case 62:
            state = 63;
            letterStream.push_back(*it);
            break;

        case 63:
            // check for string
            if (*it == '"')
                state = 64;
            else
                state = 63;
            letterStream.push_back(*it);
            break;

        case 64:
            state = 0;
            --it;
            tokens.push_back(token(letterStream, TokenType::STRING));
            letterStream = "";
            break;

        case 65:
            if (*it == 'e')
                state = 55;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 68:
            if (*it == 'i')
                state = 54;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 75: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::BEGIN));
            break;

        case 76:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 75;
            letterStream.push_back(*it);
            break;

        case 78:
            if (*it == 'd')
                state = 79;
            else if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 89;
            letterStream.push_back(*it);
            break;

        case 79:
            if (*it >= 'a' && *it <= 'z' || *it >= 'A' && *it <= 'Z')
                state = 51;
            else if (*it >= '0' && *it <= '9')
                state = 52;
            else
                state = 81;
            letterStream.push_back(*it);
            break;

        case 81: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::END));
            letterStream = "";
            break;

        case 84:
            if ((int)*it != 10)
                letterStream.push_back(*it);
            else
                state = 85;
            break;
        case 85: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::COMMENT));
            letterStream = "";
            break;
        case 86: // Only for numeric literals
            if (*it >= '0' && *it <= '9')
            {
                state = 86;
                letterStream.push_back(*it);
            }
            else
            {
                if (*it != ' ')
                    --it;
                state = 88;
            }
            break;
        case 88: // Final State: Numeric Literal
            state = 0;
            --it;
            tokens.push_back(token(letterStream, TokenType::NUMERIC_LITERAL));
            letterStream = "";
            break;
        case 89: // Final State: Identifier
                 // In DFA, there are more than 1 states for identifers
                 // that was done to make dfa more neat
                 // while coding the DFA, I only used one final state from DFA
            state = 0;
            --it;
            --it;
            tokens.push_back(token(letterStream, TokenType::IDENTIFIER));
            letterStream = "";
            break;

        case 90: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::ELSE));
            letterStream = "";
            break;

        case 91: // Final State
            state = 0;
            --it;
            letterStream = "";
            tokens.push_back(token(string(""), TokenType::PRINTLN));
            break;

        case 92: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::CALL));
            letterStream = "";
            break;

        case 93: // Final State
            state = 0;
            --it;
            letterStream = "";
            tokens.push_back(token(string(""), TokenType::CHAR));
            break;

        case 96: // Final State
            state = 0;
            --it;
            tokens.push_back(token(string(""), TokenType::IF));
            letterStream = "";
            break;

        case 98: // Final State
            state = 0;
            letterStream = "";
            --it;
            --it;
            tokens.push_back(token(letterStream, TokenType::INT));
            break;

        case 99: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(letterStream, TokenType::FOR));
            break;

        case 100: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::FUNC));
            break;

        case 101: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::COMMA));
            break;

        case 102: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::COLON));
            break;

        case 103: // Final State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token(string(""), TokenType::SEMICOLON));
            break;

        case 150: // Trap State
            state = 0;
            letterStream = "";
            --it;
            tokens.push_back(token());
        }
    }
    tokens.push_back(token(string(""), TokenType::END_OF_FILE));
}
lexer::lexer(const char filename[])
{
    // constructors: takes file name as an argument and store all
    // the contents of file into the class varible stream
    ifstream fin(filename);
    if (!fin) // if file not found
    {
        cout << "file not found" << endl;
    }
    else
    {
        char byte = 0;
        while (fin.get(byte))
        { // store file contents into stream
            if (byte != '\r')
                stream.push_back(byte);
        }
        stream.push_back(' '); // dummy spaces appended at the end
        stream.push_back(' ');
        // Tokenize function to get all the (token,lexeme) pairs
        Tokenize();
        // assigning tokens to iterator::pointer
        index = 0;
    }
}
lexer::lexer()
{
    index = -1;
}
void lexer::printRaw()
{
    // helper function to print the content of file
    vector<char>::iterator it = stream.begin();
    for (it = stream.begin(); it != stream.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
token lexer::getNextToken()
{
    // this function will return single (token,lexeme) pair on each call
    // this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
        index = index + 1;
    }
    return _token;
}
token lexer::getCurrentToken()
{
    // this function will return single (token,lexeme) pair on each call
    // this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
    }
    return _token;
}
void lexer::resetPointer()
{
    index = 0;
}
token lexer::peek(int howFar)
{
    if (howFar <= 0)
    { // peeking backward or in place is not allowed
        cout << "LexicalAnalyzer:peek:Error: non positive argument\n";
        exit(-1);
    }

    int peekIndex = index + howFar - 1;
    if (peekIndex > (tokens.size() - 1))
    {                                             // if peeking too far
        return token("", TokenType::END_OF_FILE); // return END_OF_FILE
    }
    else
        return tokens[peekIndex];
}