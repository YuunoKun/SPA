#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../../source/Tokenizer.h"

namespace UnitTesting {
	using namespace SourceProcessor;

	TEST(Tokenizer, parse_into_token_keywords) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("procedure read print call while if then else");

		std::vector<Token> test_output = tokenizer.getTokenChain();
		std::vector<TokenType> correct_output = 
		{
			PROCEDURE, 
			READ, 
			PRINT, 
			CALL, 
			WHILE, 
			IF, 
			THEN, 
			ELSE
		};

		ASSERT_EQ(test_output.size(), correct_output.size());

		for (int i = 0; i < correct_output.size(); i++) {
			ASSERT_EQ(test_output[i].getTokenType(), correct_output[i]);
		}
	}

	TEST(Tokenizer, parse_into_token_symbols) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("= + - * / % && || ! & | == != < <= > >= ( ) ; { }");

		std::vector<Token> test_output = tokenizer.getTokenChain();
		std::vector<TokenType> correct_output = 
		{
			ASSIGN,
			PLUS,
			MINUS,
			MUL,
			DIV,
			MOD,
			BOOL_AND,
			BOOL_OR,
			BOOL_NEGATE,
			BIT_AND,
			BIT_OR,
			BOOL_EQUIV,
			BOOL_NEQUIV,
			BOOL_LT,
			BOOL_LTEQ,
			BOOL_GT,
			BOOL_GTEQ,
			PARENTHESIS_OPEN,
			PARENTHESIS_CLOSE,
			TERMINATOR,
			STATEMENT_LIST_OPEN,
			STATEMENT_LIST_CLOSE 
		};

		ASSERT_EQ(test_output.size(), correct_output.size());

		for (int i = 0; i < correct_output.size(); i++) {
			ASSERT_EQ(test_output[i].getTokenType(), correct_output[i]);
		}
	}

	TEST(Tokenizer, parse_into_token_identifiers) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("prodecure is d1fferent from Procedure and pr0c3dur3");

		std::vector<Token> test_output = tokenizer.getTokenChain();
		TokenType correct_output = TokenType::IDENTIFIER;

		for (Token tk: test_output) {
			ASSERT_EQ(tk.getTokenType(), correct_output);
		}
	}

	TEST(Tokenizer, parse_into_token_constants) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("0 01 0123 9028304");

		std::vector<Token> test_output = tokenizer.getTokenChain();
		TokenType correct_output = TokenType::CONSTANT;

		for (Token tk : test_output) {
			ASSERT_EQ(tk.getTokenType(), correct_output);
		}
	}

	TEST(Tokenizer, parse_into_token_empty) {
		Tokenizer tokenizer;

		tokenizer.parseIntoTokens("");
		std::vector<Token> test_output_1 = tokenizer.getTokenChain();
		ASSERT_EQ(test_output_1.size(), 0);


		tokenizer.parseIntoTokens("                                      ");
		std::vector<Token> test_output_2 = tokenizer.getTokenChain();
		ASSERT_EQ(test_output_2.size(), 0);
	}

	// TODO: Add in more exception tests


	TEST(Tokenizer, parse_into_token_valid_1) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("procedure myProcedure {x=y+1;}");

		std::vector<Token> test_output = tokenizer.getTokenChain();
		std::vector<TokenType> correct_output = { PROCEDURE, IDENTIFIER, STATEMENT_LIST_OPEN
			, IDENTIFIER, ASSIGN, IDENTIFIER, PLUS, CONSTANT, TERMINATOR, STATEMENT_LIST_CLOSE };

		ASSERT_EQ(test_output.size(), correct_output.size());

		for (int i = 0; i < correct_output.size(); i++) {
			ASSERT_EQ(test_output[i].getTokenType(), correct_output[i]);
		}
	}

	TEST(Tokenizer, stack_operations) {
		Tokenizer tokenizer;
		tokenizer.parseIntoTokens("procedure myProcedure {x=y+1;}");



		tokenizer.initTokenStack();
		ASSERT_TRUE(tokenizer.hasToken());
		ASSERT_EQ(tokenizer.peekToken().getTokenType(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::STATEMENT_LIST_OPEN);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::ASSIGN);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::PLUS);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::CONSTANT);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::TERMINATOR);

		ASSERT_TRUE(tokenizer.hasToken());
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::STATEMENT_LIST_CLOSE);
		ASSERT_FALSE(tokenizer.hasToken());
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::INVAL);

		tokenizer.initTokenStack();
		ASSERT_EQ(tokenizer.peekToken().getTokenType(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::STATEMENT_LIST_OPEN);

		ASSERT_EQ(tokenizer.peekProbe().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::ASSIGN);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::PLUS);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::CONSTANT);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::TERMINATOR);

		ASSERT_EQ(tokenizer.peekProbe().getTokenType(), TokenType::STATEMENT_LIST_CLOSE);
		ASSERT_EQ(tokenizer.popProbe().getTokenType(), TokenType::STATEMENT_LIST_CLOSE);

		ASSERT_NO_THROW(tokenizer.popProbe().getTokenType());

		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::ASSIGN);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::PLUS);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::CONSTANT);
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::TERMINATOR);

		ASSERT_TRUE(tokenizer.hasToken());
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::STATEMENT_LIST_CLOSE);
		ASSERT_FALSE(tokenizer.hasToken());
		ASSERT_EQ(tokenizer.popToken().getTokenType(), TokenType::INVAL);
	}

}