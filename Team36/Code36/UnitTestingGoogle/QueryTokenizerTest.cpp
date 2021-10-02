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

	// Test for newline
	TEST(QueryTokenizer, NewlineTest) {
		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt s;\nSelect s";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "stmt" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });
		expected.push_back({ QueryToken::TERMINATOR, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });


		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
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
	
	// Test for identifiers
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

	TEST(QueryTokenizer, IdentifierTest2) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt +S";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken stmt_token = QueryToken(QueryToken::IDENTIFIER, "stmt");
		QueryToken plus_token = QueryToken(QueryToken::PLUS, "");
		QueryToken S_token = QueryToken(QueryToken::IDENTIFIER, "S");

		EXPECT_TRUE(output[0].token_value == stmt_token.token_value);
		EXPECT_TRUE(output[0].type == stmt_token.type);

		EXPECT_TRUE(output[1].token_value == plus_token.token_value);
		EXPECT_TRUE(output[1].type == plus_token.type);

		EXPECT_TRUE(output[2].token_value == S_token.token_value);
		EXPECT_TRUE(output[2].type == S_token.type);
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

	// Test for SUCH_THAT
	TEST(QueryTokenizer, SuchThatTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "such that ";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken such_that_token = QueryToken(QueryToken::SUCH_THAT, "");

		EXPECT_TRUE(output[0].type == such_that_token.type);

	}

	TEST(QueryTokenizer, SuchThatTest2) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select s such that Follows";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken such_that_token = QueryToken(QueryToken::SUCH_THAT, "");

		EXPECT_TRUE(output[2].type == such_that_token.type);
	}

	TEST(QueryTokenizer, SuchThatDoubleSpacingTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select s such  that Follows";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken such_token = QueryToken(QueryToken::IDENTIFIER, "such");
		QueryToken that_token = QueryToken(QueryToken::IDENTIFIER, "that");

		EXPECT_TRUE(output[2].token_value == such_token.token_value);
		EXPECT_TRUE(output[2].type == such_token.type);

		EXPECT_TRUE(output[3].token_value == that_token.token_value);
		EXPECT_TRUE(output[3].type == that_token.type);
	}

	// Test for PARENT_T
	TEST(QueryTokenizer, ParentTTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Parent*(s,_)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		QueryTokenizer query_tokenizer2;
		std::string input2 = " Parent* (s,_)";
		query_tokenizer2.parse_into_query_tokens(input2);
		std::vector<QueryToken> output2 = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::PARENT_T, "" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::WILDCARD, "" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });

		for (size_t i = 0; i<expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);

			EXPECT_TRUE(output2[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output2[i].type == expected[i].type);
		}
	}

	// Test for FOLLOW_T
	TEST(QueryTokenizer, FollowsTTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Follows*(s,_)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		QueryTokenizer query_tokenizer2;
		std::string input2 = " Follows* (s,_)";
		query_tokenizer2.parse_into_query_tokens(input2);
		std::vector<QueryToken> output2 = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::FOLLOWS_T, "" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::WILDCARD, "" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);

			EXPECT_TRUE(output2[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output2[i].type == expected[i].type);
		}
	}

	TEST(QueryTokenizer, MulAsteriskTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Parent*(s, \"Parent*8\")";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::PARENT_T, "" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Parent" });
		expected.push_back({ QueryToken::MUL, "" });
		expected.push_back({ QueryToken::CONSTANT, "8" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// Test for Arithmetic operators
	TEST(QueryTokenizer, ArithmeticTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select a pattern a ( _ , _\"v + x - y * z / t\"_)";
		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();


		QueryTokenizer query_tokenizer2;
		std::string input2 = " Select a pattern a ( _ , _\"v+x-y*z/t\"_)";
		query_tokenizer2.parse_into_query_tokens(input2);
		std::vector<QueryToken> output2 = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "a" });
		expected.push_back({ QueryToken::IDENTIFIER, "pattern" });
		expected.push_back({ QueryToken::IDENTIFIER, "a" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::WILDCARD, "" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::WILDCARD, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "v" });
		expected.push_back({ QueryToken::PLUS, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "x" });
		expected.push_back({ QueryToken::MINUS, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "y" });
		expected.push_back({ QueryToken::MUL, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "z" });
		expected.push_back({ QueryToken::DIV, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "t" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		expected.push_back({ QueryToken::WILDCARD, "" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);

			EXPECT_TRUE(output2[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output2[i].type == expected[i].type);
		}
	}
	
	// Test for attribute_names ---------------------------------------------------------------------------------------------
	
	// Test for procName
	TEST(QueryTokenizer, procNameTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Calls (p, q) with q.procName = \"Third\"";

		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::SUCH_THAT, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Calls" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "with" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::PROC_NAME, "" });
		expected.push_back({ QueryToken::EQUAL, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Third" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// test for varName
	TEST(QueryTokenizer, varNameTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Calls (p, q) with q.varName = \"Third\"";

		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::SUCH_THAT, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Calls" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "with" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::VAR_NAME, "" });
		expected.push_back({ QueryToken::EQUAL, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Third" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// test for values
	TEST(QueryTokenizer, valueTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Follows (p, q) with q.value = \"Third\"";

		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::SUCH_THAT, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Follows" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "with" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::VALUE, "" });
		expected.push_back({ QueryToken::EQUAL, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Third" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// test for stmt#
	TEST(QueryTokenizer, stmtIndexTest) {

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Follows (p, q) with q.stmt# = \"Third\"";

		query_tokenizer.parse_into_query_tokens(input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::SUCH_THAT, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Follows" });
		expected.push_back({ QueryToken::PARENTHESIS_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::COMMA, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::PARENTHESIS_CLOSE, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "with" });
		expected.push_back({ QueryToken::IDENTIFIER, "q" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::STMT_INDEX, "" });
		expected.push_back({ QueryToken::EQUAL, "" });
		expected.push_back({ QueryToken::QUOTATION_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "Third" });
		expected.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}
	
	// Invalid tests---------------------------------------------------------------------------------------------------------

	// Test catch if parenthesis still open 
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

	// Test catch if parenthesis no opening
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

	// Test catch if quotation marks missing
	TEST(QueryTokenizer, InvalidQuotationTest1) {

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

	// Test catch if quotation marks missing
	TEST(QueryTokenizer, InvalidQuotationTest2) {

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

	TEST(QueryTokenizer, InvalidSingleQuotationTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "\'procName\'";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown symbol present"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	TEST(QueryTokenizer, InvalidIdentTest) {

		try {
			QueryTokenizer query_tokenizer;
			std::string input = "1procName";
			query_tokenizer.parse_into_query_tokens(input);
			std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid Name present"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	
}