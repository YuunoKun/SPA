#pragma once

#include <string>
#include <vector>

class QueryToken
{
public:

	enum QueryTokenType {
		INVAL,

		IDENTIFIER,
		WHITESPACE,

		//PQL Punctuation
		COMMA,

		// PQL Keywords
		SELECT,
		STMT,
		READ,
		PRINT,
		CALL,
		IF,
		WHILE,
		ASSIGN,
		VARIABLE,
		CONSTANT,
		PROCEDURE,
		AND,
		PATTERN,
		SUCH_THAT,

		// PQL Variable Operators
		WILDCARD,
		QUOTATION_OPEN,
		QUOTATION_CLOSE,

		PLUS,
		MINUS,
		MUL,
		DIV,
		MOD,

		//Relationships
		// need parent type? cause will pass straight to relref class
		MODIFIES_P,
		MODIFIES_S,
		USES_P,
		USES_S,
		CALLS,
		CALLS_T,
		PARENT,
		PARENT_T,
		FOLLOWS,
		FOLLOWS_T,
		NEXT,
		NEXT_T,
		AFFECTS,
		AFFECTS_T,

		//  PQL Conditional Operators
	/*	BOOL_AND,
		BOOL_OR,
		BOOL_NEGATE,
		BIT_AND,
		BIT_OR,*/

		// PQL Relational Operators
	/*	BOOL_EQUIV,
		BOOL_NEQUIV,
		BOOL_LT,
		BOOL_LTEQ,
		BOOL_GT,
		BOOL_GTEQ,*/

		// PQL Separators
		PARENTHESIS_OPEN,
		PARENTHESIS_CLOSE,
		TERMINATOR
	};

	//Constructors
	QueryToken();

	QueryToken(QueryTokenType type_input, std::string token_value_input);

	QueryTokenType type;

	std::string token_value;
};
