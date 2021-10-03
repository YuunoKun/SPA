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

	// Test parseParameterSuchThat -------------------------------------------------------------------------------------------------------------------
	//MODIFIES_S

	TEST(PatternRelRefValidatorTest, ModifiesSSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);
		

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//MODIFIES_P
	TEST(PatternRelRefValidatorTest, ModifiesPSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ModifiesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//USES_S

	TEST(PatternRelRefValidatorTest, UsesSSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);


		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//USES_P
	TEST(PatternRelRefValidatorTest, UsesPSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, UsesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// PARENT
	TEST(PatternRelRefValidatorTest, ParentSynSynTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "s1";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentSynWildTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	
	TEST(PatternRelRefValidatorTest, ParentSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// PARENT_T
	TEST(PatternRelRefValidatorTest, ParentTSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "s1";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, ParentTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// FOLLOWS
	TEST(PatternRelRefValidatorTest, FollowsSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "s1";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// FOLLOWS_T
	TEST(PatternRelRefValidatorTest, FollowsTSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "s1";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(PatternRelRefValidatorTest, FollowsTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		PatternRelRefValidator validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
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

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
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

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
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

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	// Invalid tests -------------------------------------------------------------------------------------------------------------------------------

	// Invalid ModifiesS Test
	// test invalid Modiefies with 1st param as WILDCARD

	TEST(PatternRelRefValidatorTest, invalidModifiesSWildSynTest) {
		PatternRelRefValidator validator;

		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}
	
	TEST(PatternRelRefValidatorTest, invalidModifiesSWildWildTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidModifiesSWildIntTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}
	
	TEST(PatternRelRefValidatorTest, invalidModifiesSWildIdentTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	
	// Invalid Follows tests
	TEST(PatternRelRefValidatorTest, invalidUndeclaredSynonymFollowsTest) {
		PatternRelRefValidator validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown stmtRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	// test invalid Uses with 1st param as WILDCARD
	TEST(PatternRelRefValidatorTest, invalidWildcardUseSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		//Synonym
		//Expected
		//Synonym synonym;
		//synonym.name = "s";
		//Entity expected_declared_stmt = Entity(EntityType::STMT, synonym);
		//query.addEntity(expected_declared_stmt);

		//Entity expected_ent_wildcard = Entity(EntityType::WILD);

		//RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_declared_stmt, expected_ent_wildcard);

		//Result


		//EXPECT_TRUE(query.getRelations()[0] == expected_rel);

		try {
			validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Uses"));
		}
		catch (...) {
			// Test case should fail if not caught as runtime_error 
			FAIL();
		}
	}

	// Test invalid cases in ModifiesS with no param
	TEST(PatternRelRefValidatorTest, invalidEmptyParamModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid StmtRef arguments"));
		}
		catch (...) {
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidEmptyParamModifiesSTest2) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidEmptyParamModifiesSTest3) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidEmptyParamModifiesSTest4) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (...) {
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidUndeclaredSynonymParamModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}
	
	TEST(PatternRelRefValidatorTest, invalidUndeclaredSynonymParamModifiesSTest2) {
		PatternRelRefValidator validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown entRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidUndeclaredSynonymParamModifiesSTest3) {
		PatternRelRefValidator validator;

		Query query;
		Synonym synonym;
		synonym.name = "y";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidIdentModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "1" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown entRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidIntModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidEmptyQuotationModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });


		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown entRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidEmptyQuotationModifiesSTest2) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });



		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidSingleQuotationModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });


		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown entRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidSingleQuotationModifiesSTest2) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });


		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidTripleQuotationModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::runtime_error const& err) {
			EXPECT_EQ(err.what(), std::string("Unknown entRef"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidTripleQuotationModifiesSTest3) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });	

		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

	TEST(PatternRelRefValidatorTest, invalidStmtRefModifiesSTest) {
		PatternRelRefValidator validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });


		try {
			validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
			FAIL();
		}
		catch (std::invalid_argument const& err) {
			EXPECT_EQ(err.what(), std::string("Invalid parameters for Modifies"));
		}
		catch (std::exception& e) {
			std::cout << "Standard exception: " << e.what() << std::endl;
			// Test case should fail if not caught as invalid_argument
			FAIL();
		}
	}

}