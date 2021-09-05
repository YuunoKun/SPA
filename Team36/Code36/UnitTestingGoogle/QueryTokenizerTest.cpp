#include "pch.h"

#include "QueryTokenizer.h"
#include "QueryTokenizer.cpp"
#include "Common.h"
#include <iostream>



namespace UnitTesting {
	class QueryTokenizerTest : public ::testing::Test {
	protected:
		QueryTokenizerTest() {
		}

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}

		// Class members declared here can be used by all tests in the test suite
		// for Foo.
	};

	// Test to see if overloaded constructors work
	TEST(QueryTokenizer, ConstructorTest) {
		
		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt S";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		

		//Expected
		std::string stmt_string = "stmt";
		QueryToken stmt_token = QueryToken();
		stmt_token.type = QueryToken::IDENTIFIER;
		stmt_token.token_value = stmt_string;

		QueryToken S_token = QueryToken(QueryToken::IDENTIFIER, "S");

		
		EXPECT_TRUE(output[0].token_value == stmt_token.token_value);
		EXPECT_TRUE(output[0].type == stmt_token.type);

		EXPECT_TRUE(output[1].token_value == S_token.token_value);
		EXPECT_TRUE(output[1].type == S_token.type);
	}

	// Test for whitespace
	TEST(QueryTokenizer, WhitespaceTest) {
		QueryTokenizer query_tokenizer;


		//Expected
		QueryToken whitespace = QueryToken();

		EXPECT_TRUE(whitespace.token_value == "");
		EXPECT_TRUE(whitespace.type == QueryToken::QueryTokenType::WHITESPACE);
	}

	// Test for constants
	TEST(QueryTokenizer, ConstantTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "9 stmt 8";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken nine_token = QueryToken(QueryToken::CONSTANT, "9");
		QueryToken stmt_token = QueryToken(QueryToken::IDENTIFIER, "stmt");
		QueryToken eight_token = QueryToken(QueryToken::CONSTANT, "8");


		EXPECT_TRUE(output[0].token_value == nine_token.token_value);
		EXPECT_TRUE(output[0].type == nine_token.type);

		EXPECT_TRUE(output[1].token_value == stmt_token.token_value);
		EXPECT_TRUE(output[1].type == stmt_token.type);

		EXPECT_TRUE(output[2].token_value == eight_token.token_value);
		EXPECT_TRUE(output[2].type == eight_token.type);
	}
	
	// Test for identifiers TODO	
	TEST(QueryTokenizer, IdentifierTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt S";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken stmt_token = QueryToken(QueryToken::IDENTIFIER, "stmt");
		QueryToken S_token = QueryToken(QueryToken::IDENTIFIER, "S");

		EXPECT_TRUE(output[0].token_value == stmt_token.token_value);
		EXPECT_TRUE(output[0].type == stmt_token.type);

		EXPECT_TRUE(output[1].token_value == S_token.token_value);
		EXPECT_TRUE(output[1].type == S_token.type);
	}

	// Test for open and close parenthesis
	TEST(QueryTokenizer, ParenthesisOpenCloseTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "(procName)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken parenthesis_open_token = QueryToken(QueryToken::PARENTHESIS_OPEN, "(");
		QueryToken proc_name_token = QueryToken(QueryToken::IDENTIFIER, "procName");
		QueryToken parenthesis_close_token = QueryToken(QueryToken::PARENTHESIS_OPEN, ")");


		EXPECT_TRUE(output[0].type == QueryToken::PARENTHESIS_OPEN);

		EXPECT_TRUE(output[1].token_value == proc_name_token.token_value);
		EXPECT_TRUE(output[1].type == QueryToken::IDENTIFIER);

		EXPECT_TRUE(output[2].type == QueryToken::PARENTHESIS_CLOSE);
	}

	// Test for open and close quotation marks
	TEST(QueryTokenizer, QuotationOpenCloseTest) {
		
		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "\"procName\"";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken quotation_open_token = QueryToken(QueryToken::QUOTATION_OPEN, "\"");
		QueryToken proc_name_token = QueryToken(QueryToken::IDENTIFIER, "procName");
		QueryToken quotation_close_token = QueryToken(QueryToken::QUOTATION_CLOSE, "\"");


		EXPECT_TRUE(output[0].type == QueryToken::QUOTATION_OPEN);

		EXPECT_TRUE(output[1].token_value == proc_name_token.token_value);
		EXPECT_TRUE(output[1].type == QueryToken::QueryTokenType::IDENTIFIER);

		EXPECT_TRUE(output[2].type == QueryToken::QUOTATION_CLOSE);
	}
	// Test for terminator
	TEST(QueryTokenizer, TerminatorTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt s;";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected

		QueryToken stmt_token = QueryToken(QueryToken::IDENTIFIER, "stmt");
		QueryToken s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		QueryToken terminator_token = QueryToken(QueryToken::TERMINATOR, ";");

		EXPECT_TRUE(output[0].token_value == stmt_token.token_value);
		EXPECT_TRUE(output[0].type == QueryToken::IDENTIFIER);

		EXPECT_TRUE(output[1].token_value == s_token.token_value);
		EXPECT_TRUE(output[1].type == QueryToken::QueryTokenType::IDENTIFIER);

		EXPECT_TRUE(output[2].type == QueryToken::TERMINATOR);
	}

	// Test for wildcard
	TEST(QueryTokenizer, WildcardTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Uses (s,_)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");

		EXPECT_TRUE(output[4].type == QueryToken::WILDCARD);
	}

	// Test for comma
	TEST(QueryTokenizer, CommaTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Uses (s,_)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken wildcard_token = QueryToken(QueryToken::COMMA, ",");

		EXPECT_TRUE(output[3].type == QueryToken::COMMA);
	}


	// Test for asterisk
	// Test for Arithmetic operators


	//bigger tests

	// Invalid tests
	TEST(QueryTokenizer, InvalidParenthesisOpenTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "(procName";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("expected \')\'"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	TEST(QueryTokenizer, InvalidParenthesisCloseTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "procName)";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unexpected symbol : \')\'"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	TEST(QueryTokenizer, InvalidQuotationOpenTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "\"procName";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("missing terminating \" character"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	TEST(QueryTokenizer, InvalidQuotationCloseTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "procName\"";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("missing terminating \" character"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}



}