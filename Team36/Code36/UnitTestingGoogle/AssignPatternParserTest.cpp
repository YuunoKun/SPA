#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class AssignPatternParserTest : public ::testing::Test {
	protected:
		AssignPatternParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	// Test parseParameterPattern
	// Test ASSIGN
	TEST(AssignPatternParserTest, AssignIdentIdentTest) {
		QueryClauseParser validator;
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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentTest2) {
		QueryClauseParser validator;
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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignIdentIdentWildCardTest2) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynIdentTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignSynIdentWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardIdentTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(AssignPatternParserTest, AssignWildCardIdentWildCardTest) {
		QueryClauseParser validator;

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

		Pattern expected_pat = Pattern(expected_declared_assign, Utility::setEntRef(query, left_expr_token_chain, EntityType::VARIABLE), Utility::setExpr(right_expr_token_chain), true);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	//semantically invalid
	TEST(AssignPatternParserTest, semanticInvalidAssignSynIdentWildCardTest2) {
		QueryClauseParser validator;

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
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AssignPatternParserTest, semanticInvalidAssignExtraParams) {
		QueryClauseParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "a";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}


	TEST(AssignPatternParserTest, syntacticInvalidExprTest) {
		QueryClauseParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });

		std::vector<QueryToken> right_expr_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "b" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "assign1";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);
		
		
		EXPECT_THROW(validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain), SyntacticErrorException);
	}
	TEST(AssignPatternParserTest, syntacticInvalidExtraParams) {
		QueryClauseParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "assign1";
		Entity expected_declared_assign = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_declared_assign);

		EXPECT_THROW(validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain), SyntacticErrorException);
	}
}