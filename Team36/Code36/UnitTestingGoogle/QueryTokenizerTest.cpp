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

	// Single declaration of constant s
	// test to see if declaration and selected entity are passed correctly
	
	TEST(QueryTokenizer, QueryTokenWhitespaceTest) {
		QueryTokenizer query_tokenizer;


		//Expected
		QueryToken whitespace = QueryToken();

		EXPECT_TRUE(whitespace.token_value == "");
		EXPECT_TRUE(whitespace.type == QueryToken::QueryTokenType::WHITESPACE);

		std::cout << whitespace.token_value << " output.type ||" << '\n';
		std::cout << whitespace.type << " stmt.type ||" << '\n';
		
	}
	TEST(QueryTokenizer, QueryTokenTest) {
		QueryTokenizer query_tokenizer;

		std::string input = "stmt S";

		query_tokenizer.parse_into_query_tokens(input);

		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
		

		//Expected
		std::string stmt_string = "stmt";
		std::string S_string = "S";
		QueryToken stmt_token = QueryToken();
		stmt_token.type = QueryToken::IDENTIFIER;
		stmt_token.token_value = stmt_string;

		QueryToken S_token = QueryToken(QueryToken::IDENTIFIER, S_string);
		/*S_token.type = QueryToken::IDENTIFIER;
		S_token.token_value = S_string;*/
		
		EXPECT_TRUE(output[0].token_value == stmt_token.token_value);
		std::cout << "||" << output[0].token_value << "||output.type ||" << '\n';
		std::cout << "||" << stmt_token.token_value << "||stmt.type ||" << '\n';
		EXPECT_TRUE(output[0].type == QueryToken::IDENTIFIER);

		

		EXPECT_TRUE(output[1].token_value == S_token.token_value);
		EXPECT_TRUE(output[1].type == QueryToken::QueryTokenType::IDENTIFIER);

	}

}