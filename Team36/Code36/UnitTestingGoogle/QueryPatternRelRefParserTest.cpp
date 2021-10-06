#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class QueryPatternRelRefParserTest : public ::testing::Test {
	protected:
		QueryPatternRelRefParserTest() {
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
	TEST(QueryPatternRelRefParserTest, setStmtRefTest) {
		QueryPatternRelRefParser validator;

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
	TEST(QueryPatternRelRefParserTest, setEntRefTest) {
		QueryPatternRelRefParser validator;
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
	TEST(QueryPatternRelRefParserTest, setExprTest) {
		QueryPatternRelRefParser validator;

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

	TEST(QueryPatternRelRefParserTest, ModifiesSSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);
		

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
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
	TEST(QueryPatternRelRefParserTest, ModifiesPSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ModifiesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
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

	TEST(QueryPatternRelRefParserTest, UsesSSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);


		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::CONSTANT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
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
	TEST(QueryPatternRelRefParserTest, UsesPSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, UsesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::VARIABLE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
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
	TEST(QueryPatternRelRefParserTest, ParentSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentSynWildTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	
	TEST(QueryPatternRelRefParserTest, ParentSynIntTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentWildSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// PARENT_T
	TEST(QueryPatternRelRefParserTest, ParentTSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTSynIntTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTWildSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, ParentTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::PARENT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// FOLLOWS
	TEST(QueryPatternRelRefParserTest, FollowsSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsSynIntTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsWildSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::FOLLOWS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// FOLLOWS_T
	TEST(QueryPatternRelRefParserTest, FollowsTSynSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTSynIntTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTWildSynTest) {
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
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "3");

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, FollowsTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::CONSTANT, "3");

		Entity expected_stmt_2 = Entity(EntityType::CONSTANT, "4");

		RelRef expected_rel = RelRef(RelType::FOLLOWS_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::FOLLOWS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//CALLS
	TEST(QueryPatternRelRefParserTest, CallsSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "p1";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p1" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::PROCEDURE, "main");

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsWildSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);
		
		Synonym synonym2;
		synonym2.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsWildWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsWildIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);

		Entity expected_2 = Entity(EntityType::PROCEDURE, "main");

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Synonym synonym;
		synonym.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_2);


		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::WILD, "");



		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsIdentIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::PROCEDURE, "program");

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//CALLS_T
	TEST(QueryPatternRelRefParserTest, CallsTSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_stmt_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "p1";
		Entity expected_stmt_2 = Entity(EntityType::PROCEDURE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p1" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::PROCEDURE, "main");

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTWildSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);

		Synonym synonym2;
		synonym2.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallTsWildWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTWildIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::WILD);

		Entity expected_2 = Entity(EntityType::PROCEDURE, "main");

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Synonym synonym;
		synonym.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_2);


		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::WILD, "");



		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(QueryPatternRelRefParserTest, CallsTIdentIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::PROCEDURE, "program");

		RelRef expected_rel = RelRef(RelType::CALLS_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	// Test parseParameterPattern
	// Test ASSIGN
	TEST(QueryPatternRelRefParserTest, parseParameterPatternAssignTest) {
		QueryPatternRelRefParser validator;
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

		Pattern expected_pat = Pattern(expected_declared_assign, validator.setEntRef(query, left_expr_token_chain), validator.setExpr(right_expr_token_chain), false);

		//Result
		validator.parseParameterPattern(query, expected_declared_assign, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getPattern() == expected_pat);
	}

	TEST(QueryPatternRelRefParserTest, parseParameterPatternAssignWildCardTest) {
		QueryPatternRelRefParser validator;

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

	TEST(QueryPatternRelRefParserTest, parseParameterPatternAssignSingleWildCardTest) {
		QueryPatternRelRefParser validator;

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

	// Invalid tests -------------------------------------------------------------------------------------------------------------------------------

	// Semantically Invalid Test -------------------------------------------------------------------------------------------------------------------
	
	// Follows tests

	TEST(QueryPatternRelRefParserTest, semanticInvalidFollowsIdentWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ident" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidFollowsVarWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSIntIntTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}

	// ModifiesS, ModifiesP Test
	// test semantically invalid Modiefies with 1st param as WILDCARD
	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSWildSynTest) {
		QueryPatternRelRefParser validator;

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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}
	
	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSWildWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSWildIntTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}
	
	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSWildIdentTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}
	
	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesSVarVarTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "v";
		Entity expected_1 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v1";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		//RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v1" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidModifiesPProcProcTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "p1";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_2);

		//RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SemanticErrorException);
	}

	// UsesS, UsesP Test
	// test semantically invalid Uses with 1st param as WILDCARD
	TEST(QueryPatternRelRefParserTest, semanticInvalidUsesSWildSynTest) {
		QueryPatternRelRefParser validator;

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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidUsesSWildWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidUsesSWildIntTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidUsesSWildIdentTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain), SemanticErrorException);
	}

	//Calls test
	TEST(QueryPatternRelRefParserTest, semanticInvalidCallsProcVarTest) {
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

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SemanticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidCallsVarProcTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "v";
		Entity expected_1 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_1);
		
		Synonym synonym2;
		synonym2.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		query.addEntity(expected_2);


		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SemanticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidCallsProcStmtTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "s";
		Entity expected_2 = Entity(EntityType::STMT, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SemanticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, semanticInvalidCallsProcIntTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::CONSTANT, "5");

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		
		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SemanticErrorException);

	}
	// Syntactically Invalid Test-----------------------------------------------------------------------------------------------------------------------
	
	// Follows tests

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsUndeclaredSynTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsUndeclaredSynTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });
		
		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsUndeclaredSynTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "y";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsEmptyParamTest) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsEmptyParamTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsEmptyParamTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		
		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidFollowsEmptyParamTest4) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::FOLLOWS, temp_token_chain), SyntacticErrorException);
	}

	//ModifiesS/ModifiesP test

	/*TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSInvalidIdentTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "1" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}*/

	/*TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSInvalidIdentTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}*/

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSEmptyQuotationTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSEmptyQuotationTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	//Should be handled by Query Tokenizer
	/*TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSMissingQuotationTest1) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}*/

	//Should be handled by Query Tokenizer
	//TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSMissingQuotationTest2) {
	//	QueryPatternRelRefParser validator;
	//	Query query;
	//	Synonym synonym;
	//	synonym.name = "v";
	//	Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
	//	query.addEntity(declared_stmt);
	//	std::vector<QueryToken> temp_token_chain;
	//	temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
	//	temp_token_chain.push_back({ QueryToken::COMMA, "" });
	//	temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
	//	EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	//}

	//Should be handled by Query Tokenizer

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSMissingQuotationTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	//Should be handled by Query Tokenizer
	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSMissingQuotationTest4) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSUndeclaredSynTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesSUndeclaredSynTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesUndeclaredSynTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "y";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesEmptyParamTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesEmptyParamTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesEmptyParamTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidModifiesEmptyParamTest4) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	//Calls test
	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyQuotationTest) {

		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::PROCEDURE, "main");

		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyQuotationTest2) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");
		
		Synonym synonym;
		synonym.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_2);


		RelRef expected_rel = RelRef(RelType::CALLS, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsUndeclaredSynTest) {
		QueryPatternRelRefParser validator;

		Query query;
		
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsUndeclaredSynTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsUndeclaredSynTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "y";
		Entity declared_stmt = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyParamTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyParamTest2) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "p";
		Entity declared_stmt = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyParamTest3) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "p";
		Entity declared_stmt = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);

	}

	TEST(QueryPatternRelRefParserTest, syntaticInvalidCallsEmptyParamTest4) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}
}