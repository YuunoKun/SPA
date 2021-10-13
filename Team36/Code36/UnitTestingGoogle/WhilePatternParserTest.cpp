#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class WhilePatternParserTest : public ::testing::Test {
	protected:
		WhilePatternParserTest() {
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

	//TEST(WhilePatternParserTest, WhileIdentWildCardTest) {
	//	QueryPatternRelRefParser validator;

	//	Query query;

	//	std::vector<QueryToken> temp_token_chain;

	//	temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
	//	temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
	//	temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
	//	temp_token_chain.push_back({ QueryToken::COMMA, "" });
	//	temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	std::vector<QueryToken> left_expr_token_chain;
	//	left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
	//	left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
	//	left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

	//	std::vector<QueryToken> right_expr_token_chain;
	//	right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	//Synonym
	//	//Expected
	//	Synonym synonym;
	//	synonym.name = "w";
	//	Entity expected_declared_syn = Entity(EntityType::WHILE, synonym);
	//	query.addEntity(expected_declared_syn);

	//	//Entity expected_ent_wildcard = Entity(EntityType::WILD);

	//	Pattern expected_pat = Pattern(expected_declared_syn, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

	//	//Result
	//	validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

	//	EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	//}

	//TEST(WhilePatternParserTest, WhileSynWildCardTest) {
	//	QueryPatternRelRefParser validator;

	//	Query query;

	//	std::vector<QueryToken> temp_token_chain;

	//	temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
	//	temp_token_chain.push_back({ QueryToken::COMMA, "" });
	//	temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	std::vector<QueryToken> left_expr_token_chain;
	//	left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

	//	std::vector<QueryToken> right_expr_token_chain;
	//	right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	//Expected
	//	Synonym synonym;
	//	synonym.name = "w";
	//	Entity expected_declared_syn = Entity(EntityType::WHILE, synonym);
	//	query.addEntity(expected_declared_syn);

	//	Synonym synonym2;
	//	synonym2.name = "var";
	//	Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
	//	query.addEntity(expected_2);

	//	Pattern expected_pat = Pattern(expected_declared_syn, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

	//	//Result
	//	validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

	//	EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	//}

	//TEST(WhilePatternParserTest, WhileWildCardWildCardTest) {
	//	QueryPatternRelRefParser validator;

	//	Query query;

	//	std::vector<QueryToken> temp_token_chain;

	//	temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
	//	temp_token_chain.push_back({ QueryToken::COMMA, "" });
	//	temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	std::vector<QueryToken> left_expr_token_chain;
	//	left_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	std::vector<QueryToken> right_expr_token_chain;
	//	right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

	//	//Expected
	//	Synonym synonym;
	//	synonym.name = "w";
	//	Entity expected_declared_syn = Entity(EntityType::WHILE, synonym);
	//	query.addEntity(expected_declared_syn);

	//	Pattern expected_pat = Pattern(expected_declared_syn, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

	//	//Result
	//	validator.parseParameterPattern(query, expected_declared_syn, temp_token_chain);

	//	EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	//}

}