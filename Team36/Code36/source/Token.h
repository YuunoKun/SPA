/*
Defining SIMPLE tokens for parsing.

*/

#pragma once

#include <string>
#include <vector>

namespace SourceProcessor {

	typedef enum TOKEN_TYPE {
		INVAL,

		IDENTIFIER,
		WHITESPACE,
		CONSTANT,

		// SIMPLE Keyword
		PROCEDURE,
		READ,
		PRINT,
		CALL,
		IF,
		WHILE,
		THEN,
		ELSE,

		// SIMPLE Operators
		ASSIGN,

		// SIMPLE Operators
		PLUS,
		MINUS,
		MUL,
		DIV,
		MOD,

		//  SIMPLE Conditional Operators
		BOOL_AND,
		BOOL_OR,
		BOOL_NEGATE,
		BIT_AND,
		BIT_OR,

		// SIMPLE Relational Operators
		BOOL_EQUIV,
		BOOL_NEQUIV,
		BOOL_LT,
		BOOL_LTEQ,
		BOOL_GT,
		BOOL_GTEQ,

		// SIMPLE Separators
		PARENTHESIS_OPEN,
		PARENTHESIS_CLOSE,
		TERMINATOR,
		STATEMENT_LIST_OPEN,
		STATEMENT_LIST_CLOSE
	} TokenType;


	static const char* tokenTypeStrings[] = {
		"INVAL",

		"IDENTIFIER",
		"WHITESPACE",
		"CONSTANT",

		"PROCEDURE",
		"READ",
		"PRINT",
		"CALL",
		"IF",
		"WHILE",
		"THEN",
		"ELSE",

		"ASSIGN",

		"PLUS",
		"MINUS",
		"MUL",
		"DIV",
		"MOD",

		"BOOL_AND",
		"BOOL_OR",
		"BOOL_NEGATE",
		"BIT_AND",
		"BIT_OR",

		"BOOL_EQUIV",
		"BOOL_NEQUIV",
		"BOOL_LT",
		"BOOL_LTEQ",
		"BOOL_GT",
		"BOOL_GTEQ",

		"PARENTHESIS_OPEN",
		"PARENTHESIS_CLOSE",
		"TERMINATOR",
		"STATEMENT_LIST_OPEN",
		"STATEMENT_LIST_CLOSE"
	};


	class Token {
	public : 

		// Constructor
		Token();

		// Constructor with arguments
		Token(TokenType, std::string);

		TokenType get_token_type();

		void set_token_type(TokenType);

		std::string &get_token_value();

		void set_token_value(std::string);

		std::string to_string();

	private:
		TokenType m_type;
		std::string m_token_value;
	};
} // namespace SourceProcessor

