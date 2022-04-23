#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// all your tokens goes here
enum class TokenType
{
	END_OF_FILE = 0,
	ERROR,
	IDENTIFIER,
	FUNC,
	INT,
	CHAR,
	CALL,
	IF,
	ELIF,
	ELSE,
	FOR,
	PRINT,
	PRINTLN,
	RETURN,
	IN,
	RO,
	NE,
	ASSIGNMENT_OPERATOR,
	ARITHEMATIC_OPERATOR,
	NUMERIC_LITERAL,
	CHARACTER_LITERAL,
	COMMA,
	SEMICOLON,
	COLON,
	BEGIN,
	END,
	COMMENT,
	STRING
};
// structure of a token
struct token
{
	string lexeme;
	TokenType tokenType; // enum type
	// constructor
	token(string lexeme, TokenType tokenType);
	// constructor default
	token();
	void Print();
};
class lexer
{
	vector<char> stream;  // used for storing file sample_code.cc content
	vector<token> tokens; // vector to store all (tokens,lexeme) pairs
	void Tokenize();	  // populates tokens vector
	int index;

public:
	lexer();
	lexer(const char filename[]);
	void printRaw();				 // just print everything as it is present in file
	token getNextToken();			 // get next token
	token getCurrentToken();		 // get next token
	void resetPointer();			 // reset pointer to start getting tokens from start
	int getCurrentPointer();		 // get where current pointer in tokens vector is
	void setCurrentPointer(int pos); // move current pointer to wherever
	token peek(int);				 // peek the next token
};

#endif // !_LEXER_H
