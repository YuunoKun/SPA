/*
Defining SIMPLE tokens for parsing.

*/

#pragma once


namespace Token {

	enum TokenType {
		// Does not exist
		INVAL,

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
		BOOL_NEQ,

		// SIMPLE Relational Operators
		BOOL_EQUIV,
		BOOL_LESSTHAN,
		BOOL_LESSOREQ,
		BOOL_LARGERTHAN,
		BOOL_LARGEROREQ

	};





}

