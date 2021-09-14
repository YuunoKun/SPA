#include "pch.h"

#include "PatternRelRefValidator.h"
#include "Common.h"
#include <iostream>



namespace UnitTesting {
	class PatternRelRefValidatorTest : public ::testing::Test {
	protected:
		PatternRelRefValidatorTest() {
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

	// Test setStmtRef
	// 3 diff params 
	TEST(PatternRelRefValidatorTest, setStmtRefTest) {
		PatternRelRefValidator validator;

		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		//Synonym
		//Result
		QueryToken stmt_s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		Entity ent_stmt = validator.setStmtRef(query, stmt_s_token);

		EXPECT_TRUE(ent_stmt == declared_stmt);

		/*============================================================================*/

		//WILDCARD
		//Result
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");
		Entity ent_wildacrd = validator.setStmtRef(query, wildcard_token);

		//Expected
		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		EXPECT_TRUE(ent_wildacrd == expected_ent_wildcard);

		/*============================================================================*/

		//CONSTANT
		//Result
		QueryToken constant_token = QueryToken(QueryToken::CONSTANT, "9");
		Entity ent_constant = validator.setStmtRef(query, constant_token);

		////Expected
		Entity expected_ent_constant = Entity(EntityType::CONSTANT, "9");

		EXPECT_TRUE(ent_constant == expected_ent_constant);
	}

	// Test setEntRef
	TEST(PatternRelRefValidatorTest, setEntRefTest) {
		PatternRelRefValidator validator;
		Query query;
		std::vector<QueryToken> temp_token_chain;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_declared_stmt);
		
		//Result
		QueryToken stmt_s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		temp_token_chain.push_back(stmt_s_token);
		Entity ent_stmt = validator.setEntRef(query, temp_token_chain);

		EXPECT_TRUE(ent_stmt == expected_declared_stmt);

		/*============================================================================*/
		temp_token_chain.clear();

		//WILDCARD
		//Result
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");
		temp_token_chain.push_back(wildcard_token);
		Entity ent_wildacrd = validator.setEntRef(query, temp_token_chain);

		//Expected
		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		EXPECT_TRUE(ent_wildacrd == expected_ent_wildcard);

		/*============================================================================*/
		temp_token_chain.clear();

		//‘"’ IDENT ‘"’
		//Result
		QueryToken quotation_open_token = QueryToken(QueryToken::QUOTATION_OPEN, "");
		QueryToken proc_name_token = QueryToken(QueryToken::IDENTIFIER, "procName");
		QueryToken quotation_close_token = QueryToken(QueryToken::QUOTATION_CLOSE, "");
		temp_token_chain.push_back(quotation_open_token);
		temp_token_chain.push_back(proc_name_token);
		temp_token_chain.push_back(quotation_close_token);

		Entity ent_constant = validator.setEntRef(query, temp_token_chain);

		//Expected
		Entity expected_ent_constant = Entity(EntityType::VARIABLE, "procName");

		EXPECT_TRUE(ent_constant == expected_ent_constant);
	}

	// Test setExpr
	TEST(PatternRelRefValidatorTest, setExprTest) {
		PatternRelRefValidator validator;

		std::vector<QueryToken> temp_token_chain;

		//Expected
		expr expect_expr = "s";

		//Result
		QueryToken stmt_s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		temp_token_chain.push_back(stmt_s_token);
		expr result_expr = validator.setExpr(temp_token_chain);

		EXPECT_TRUE(expect_expr == result_expr);
	}

	// Test parseParameterSuchThat
	//MODIFIES_S
	TEST(PatternRelRefValidatorTest, parseParameterSuchThatModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_declared_stmt);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_declared_stmt, expected_ent_wildcard);

		//Result
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		
		EXPECT_TRUE(query.getRelations()[0] == expected_rel);
	}

	// Test PARENT
	TEST(PatternRelRefValidatorTest, parseParameterSuchThatParentTest) {
		PatternRelRefValidator validator;
		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_declared_stmt);

		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_declared_stmt, expected_ent_wildcard);

		//Result
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getRelations()[0] == expected_rel);
	}

	// Test parseParameterPattern
	// Test ASSIGN
	TEST(PatternRelRefValidatorTest, parseParameterPatternAssignTest) {
		PatternRelRefValidator validator;
		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });



		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });


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

		EXPECT_TRUE(query.getPatterns()[0] == expected_pat);
	}

	TEST(PatternRelRefValidatorTest, parseParameterPatternAssignWildCardTest) {
		PatternRelRefValidator validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });




		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;
		right_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });


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

		EXPECT_TRUE(query.getPatterns()[0] == expected_pat);
	}

	TEST(PatternRelRefValidatorTest, parseParameterPatternAssignSingleWildCardTest) {
		PatternRelRefValidator validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });




		std::vector<QueryToken> left_expr_token_chain;
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		left_expr_token_chain.push_back({ QueryToken::IDENTIFIER, "procName" });
		left_expr_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		std::vector<QueryToken> right_expr_token_chain;


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

		EXPECT_TRUE(query.getPatterns()[0] == expected_pat);
	}

}