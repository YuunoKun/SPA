#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class AssignPatternParserTest : public ::testing::Test {
	protected:
		AssignPatternParserTest() {
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

	// Test parseParameterPattern
	// Test ASSIGN
	TEST(AssignPatternParserTest, AssignIdentIdentTest) {
		QueryPatternRelRefParser validator;
		Query query;

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentTest2) {
		QueryPatternRelRefParser validator;
		Query query;

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentWildCardTest2) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynIdentTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynIdentWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardIdentTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardIdentWildCardTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	//semantically invalid
	TEST(AssignPatternParserTest, semanticInvalidAssignSynIdentWildCardTest2) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::PLUS, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		right_expr_token_chain.push_back({ QueryToken::PLUS, "" });
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		right_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		right_expr_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Synonym synonym2;
		synonym2.name = "procName";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		query.addEntity(expected_2);

		EXPECT_THROW(validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain), SemanticErrorException);
	}
}