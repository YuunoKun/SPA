#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include "../../source/Tokenizer.h"

namespace UnitTesting {
	using namespace SourceProcessor;

	TEST(Tokenizer, parse_into_token_keywords) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("procedure read print call while if then else");

		std::vector<Token> test_output = tokenizer.get_token_chain();
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
			ASSERT_EQ(test_output[i].get_token_type(), correct_output[i]);
		}
	}

	TEST(Tokenizer, parse_into_token_symbols) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("= + - * / % && || ! & | == != < <= > >= ( ) ; { }");

		std::vector<Token> test_output = tokenizer.get_token_chain();
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
			ASSERT_EQ(test_output[i].get_token_type(), correct_output[i]);
		}
	}

	TEST(Tokenizer, parse_into_token_identifiers) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("prodecure is d1fferent from Procedure and pr0c3dur3");

		std::vector<Token> test_output = tokenizer.get_token_chain();
		TokenType correct_output = TokenType::IDENTIFIER;

		for (Token tk: test_output) {
			ASSERT_EQ(tk.get_token_type(), correct_output);
		}
	}

	TEST(Tokenizer, parse_into_token_constants) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("0 01 0123 9028304");

		std::vector<Token> test_output = tokenizer.get_token_chain();
		TokenType correct_output = TokenType::CONSTANT;

		for (Token tk : test_output) {
			ASSERT_EQ(tk.get_token_type(), correct_output);
		}
	}

	TEST(Tokenizer, parse_into_token_empty) {
		Tokenizer tokenizer;

		tokenizer.parse_into_tokens("");
		std::vector<Token> test_output_1 = tokenizer.get_token_chain();
		ASSERT_EQ(test_output_1.size(), 0);


		tokenizer.parse_into_tokens("                                      ");
		std::vector<Token> test_output_2 = tokenizer.get_token_chain();
		ASSERT_EQ(test_output_2.size(), 0);
	}

	// TODO: Add in more exception tests


	TEST(Tokenizer, parse_into_token_valid_1) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("procedure myProcedure {x=y+1;}");

		std::vector<Token> test_output = tokenizer.get_token_chain();
		std::vector<TokenType> correct_output = { PROCEDURE, IDENTIFIER, STATEMENT_LIST_OPEN
			, IDENTIFIER, ASSIGN, IDENTIFIER, PLUS, CONSTANT, TERMINATOR, STATEMENT_LIST_CLOSE };

		ASSERT_EQ(test_output.size(), correct_output.size());

		for (int i = 0; i < correct_output.size(); i++) {
			ASSERT_EQ(test_output[i].get_token_type(), correct_output[i]);
		}
	}

	TEST(Tokenizer, stack_operations) {
		Tokenizer tokenizer;
		tokenizer.parse_into_tokens("procedure myProcedure {x=y+1;}");



		tokenizer.init_token_stack();
		ASSERT_TRUE(tokenizer.has_token());
		ASSERT_EQ(tokenizer.peek_token().get_token_type(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::PROCEDURE);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::STATEMENT_LIST_OPEN);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::ASSIGN);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::IDENTIFIER);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::PLUS);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::CONSTANT);
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::TERMINATOR);

		ASSERT_TRUE(tokenizer.has_token());
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::STATEMENT_LIST_CLOSE);
		ASSERT_FALSE(tokenizer.has_token());
		ASSERT_EQ(tokenizer.pop_token().get_token_type(), TokenType::INVAL);
	}

}