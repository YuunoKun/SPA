#include "pch.h"

#include "QueryTokenizer.h"
#include "Common.h"
#include <iostream>



namespace UnitTesting {
	class QueryTokenizerTest : public ::testing::Test {
	protected:
		QueryTokenizerTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	// Test to see if overloaded constructors work
	TEST(QueryTokenizer, ConstructorTest) {
		Query query;
		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt S";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt s;\nSelect s";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "9 stmt 8";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt S";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt +S";
		query_tokenizer.tokenize(query, input);
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

	//Test prog_line
	TEST(QueryTokenizer, prog_lineTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		query_tokenizer.tokenize(query, "stmt s; prog_line n; ");
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "stmt" });
		expected.push_back({ QueryToken::IDENTIFIER, "s" });
		expected.push_back({ QueryToken::TERMINATOR, "" });
		expected.push_back({ QueryToken::PROG_LINE, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "n" });
		expected.push_back({ QueryToken::TERMINATOR, "" });


		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// Test for open and close parenthesis
	TEST(QueryTokenizer, ParenthesisOpenCloseTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "(procName)";
		query_tokenizer.tokenize(query, input);
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

	//Test for Tuple Open Close
	TEST(QueryTokenizer, TupleOpenCloseTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		query_tokenizer.tokenize(query, "<p.stmt#>");
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::TUPLE_OPEN, "" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::STMT_INDEX, "" });
		expected.push_back({ QueryToken::TUPLE_CLOSE, "" });

		for (size_t i = 0; i < expected.size(); i++) {
			EXPECT_TRUE(output[i].token_value == expected[i].token_value);
			EXPECT_TRUE(output[i].type == expected[i].type);
		}
	}

	// Test for open and close quotation marks
	TEST(QueryTokenizer, QuotationOpenCloseTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "\"procName\"";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "stmt s;";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Uses (s,_)";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");

		EXPECT_TRUE(output[4].type == QueryToken::WILDCARD);
	}

	// Test for comma
	TEST(QueryTokenizer, CommaTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Uses (s,_)";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken wildcard_token = QueryToken(QueryToken::COMMA, ",");

		EXPECT_TRUE(output[3].type == QueryToken::COMMA);
	}

	// Test for SUCH_THAT
	TEST(QueryTokenizer, SuchThatTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "such that ";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken such_that_token = QueryToken(QueryToken::SUCH_THAT, "");

		EXPECT_TRUE(output[0].type == such_that_token.type);

	}

	TEST(QueryTokenizer, SuchThatTest2) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select s such that Follows";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		QueryToken such_that_token = QueryToken(QueryToken::SUCH_THAT, "");

		EXPECT_TRUE(output[2].type == such_that_token.type);
	}

	TEST(QueryTokenizer, SuchThatDoubleSpacingTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select s such  that Follows";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Parent*(s,_)";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		QueryTokenizer query_tokenizer2;
		std::string input2 = " Parent* (s,_)";
		query_tokenizer2.tokenize(query, input2);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Follows*(s,_)";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		QueryTokenizer query_tokenizer2;
		std::string input2 = " Follows* (s,_)";
		query_tokenizer2.tokenize(query, input2);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Parent*(s, \"Parent*8\")";
		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select a pattern a ( _ , _\"v + x - y * z / t\"_)";
		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();


		QueryTokenizer query_tokenizer2;
		std::string input2 = " Select a pattern a ( _ , _\"v+x-y*z/t\"_)";
		query_tokenizer2.tokenize(query, input2);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Calls (p, q) with q.procName = \"Third\"";

		query_tokenizer.tokenize(query, input);
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

	TEST(QueryTokenizer, multipleprocNameTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p.procName such that Calls (p, q) with q.procName = \"Third\"";

		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();

		//Expected
		std::vector<QueryToken> expected;
		expected.push_back({ QueryToken::IDENTIFIER, "Select" });
		expected.push_back({ QueryToken::IDENTIFIER, "p" });
		expected.push_back({ QueryToken::DOT, "" });
		expected.push_back({ QueryToken::PROC_NAME, "" });
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Calls (p, q) with q.varName = \"Third\"";

		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Follows (p, q) with q.value = \"Third\"";

		query_tokenizer.tokenize(query, input);
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
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Follows (p, q) with q.stmt# = \"Third\"";

		query_tokenizer.tokenize(query, input);
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
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "(procName";
		
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);
	}

	// Test catch if parenthesis no opening
	TEST(QueryTokenizer, InvalidParenthesisCloseTest) {
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "procName)";

		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);

	}

	//Test for Tuple still Open
	TEST(QueryTokenizer, InvalidTupleOpenTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "<p.stmt#";
		
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);

	}

	//Test for Tuple no Open
	TEST(QueryTokenizer, InvalidTupleCloseTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "p.stmt#>";

		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);

	}

	// Test catch if quotation marks missing
	TEST(QueryTokenizer, InvalidQuotationTest1) {
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "\"procName";
		
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);
	}

	// Test catch if quotation marks missing
	TEST(QueryTokenizer, InvalidQuotationTest2) {
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "procName\"";
		
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);
	}

	TEST(QueryTokenizer, InvalidSingleQuotationTest) {
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "\'procName\'";
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);

	}

	TEST(QueryTokenizer, InvalidIdentTest) {
		Query query;

		QueryTokenizer query_tokenizer;
		std::string input = "1procName";
		EXPECT_THROW(query_tokenizer.tokenize(query, input), SyntacticErrorException);

	}

	TEST(QueryTokenizer, InvalidSemanticLeadingZerosTest) {
		Query query;

		//Result
		QueryTokenizer query_tokenizer;
		std::string input = "Select p such that Follows (09, q)";

		query_tokenizer.tokenize(query, input);
		std::vector<QueryToken> output = query_tokenizer.get_query_token_chain();


		EXPECT_TRUE(Utility::checkIsSemanticError(query));

	}


}