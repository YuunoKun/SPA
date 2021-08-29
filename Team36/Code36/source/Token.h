/*
Defining SIMPLE tokens for parsing.

*/

#pragma once

#include <string>
#include <vector>

namespace Tokenizer {

	enum TokenType {
		// Does not exist
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
	};


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
		"BOOL_GTEQ"

		"PARENTHESIS_OPEN",
		"PARENTHESIS_CLOSE",
		"TERMINATOR",
		"STATEMENT_LIST_OPEN",
		"STATEMENT_LIST_CLOSE"
	};


	class Token {
	public : 
		enum TokenType m_type {WHITESPACE};
		std::string m_token_value{" "};

		std::string to_string();
	};
}

