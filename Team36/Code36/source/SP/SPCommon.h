#pragma once

#include "Common/Common.h"

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


	constexpr char* TOKENVALUE_PLACEHOLDER = "";

	// Valid keywords for tokenizer
	constexpr char* KEYWORD_PROCEDURE = "procedure";
	constexpr char* KEYWORD_READ = "read";
	constexpr char* KEYWORD_PRINT = "print";
	constexpr char* KEYWORD_CALL = "call";
	constexpr char* KEYWORD_IF = "if";
	constexpr char* KEYWORD_WHILE = "while";
	constexpr char* KEYWORD_THEN = "then";
	constexpr char* KEYWORD_ELSE = "else";


	// Valid symbols for tokenizer
	constexpr char SYMBOL_SPACE = ' ';
	constexpr char SYMBOL_TAB = '	';
	constexpr char SYMBOL_PLUS_SIGN = '+';
	constexpr char SYMBOL_MINUS_SIGN = '-';
	constexpr char SYMBOL_ASTERISK = '*';
	constexpr char SYMBOL_SLASH = '/';
	constexpr char SYMBOL_PERCENT_SIGN = '%';
	constexpr char SYMBOL_EQUAL_SIGN = '=';
	constexpr char SYMBOL_AMPERSAND = '&';
	constexpr char SYMBOL_VERTICAL_BAR = '|';
	constexpr char SYMBOL_EXCLAMATION_MARK = '!';
	constexpr char SYMBOL_LESS_THAN_SIGN = '<';
	constexpr char SYMBOL_GREATER_THAN_SIGN = '>';

	constexpr char SYMBOL_LEFT_PARENTHESIS = '(';
	constexpr char SYMBOL_RIGHT_PARENTHESIS = ')';
	constexpr char SYMBOL_LEFT_BRACE = '{';
	constexpr char SYMBOL_RIGHT_BRACE = '}';
	constexpr char SYMBOL_SEMICOLON = ';';



	const stmt_index STMT_DEFAULT_DIRECT_PARENT = 0;
	const stmt_index STMT_DEFAULT_STMT_LIST_NO = 0;
	const stmt_index DE_INIT_STMT_NO = 0;
	const proc_index DE_INIT_PROC_NO = 0;
	const stmt_index DE_INIT_STMT_LIST_NO = 1;
	const stmt_index NON_EXISTING_STMT_NO = 0;
	const stmt_index NON_EXISTING_PROC_NO = 0;

	// Array index start from 0, but stmt and proc start from OFFSET
	const size_t OFFSET = 1;

	const size_t IF_STMT_BRANCH_NO = 2;

}// namespace SourceProcessor
