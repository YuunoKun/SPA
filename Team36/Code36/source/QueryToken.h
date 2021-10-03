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

		// PQL Keywords
		SELECT,
		STMT,
		READ,
		PRINT,
		CALL,
		VARIABLE,
		CONSTANT,
		PROCEDURE,
		AND,
		PATTERN,
		SUCH_THAT,
		PROG_LINE,
		ATTR_NAME,
		PROC_NAME,
		VAR_NAME,
		VALUE,
		STMT_INDEX,
		WITH,

		//Relationships
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

		//Pattern
		ASSIGN,
		IF,
		WHILE,

		// PQL symbols
		WILDCARD,
		QUOTATION_OPEN,
		QUOTATION_CLOSE,
		COMMA,
		PARENTHESIS_OPEN,
		PARENTHESIS_CLOSE,
		TUPLE_OPEN,
		TUPLE_CLOSE,
		EQUAL,
		HASH,
		DOT,
		TERMINATOR,

		// PQL expr opertators
		PLUS,
		MINUS,
		MUL,
		DIV,
		MOD
	};

	//Constructors
	QueryToken();

	QueryToken(QueryTokenType type_input, std::string token_value_input);

	QueryTokenType type;

	std::string token_value;
};