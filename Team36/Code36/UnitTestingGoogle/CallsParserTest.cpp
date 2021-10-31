#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class CallsParserTest : public ::testing::Test {
	protected:
		CallsParserTest() {
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

	//CALLS
	TEST(CallsParserTest, CallsSynSynTest) {
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

	TEST(CallsParserTest, CallsSynWildTest) {
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

	TEST(CallsParserTest, CallsSynIdentTest) {
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

	TEST(CallsParserTest, CallsWildSynTest) {
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

	TEST(CallsParserTest, CallsWildWildTest) {
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

	TEST(CallsParserTest, CallsWildIdentTest) {
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

	TEST(CallsParserTest, CallsIdentSynTest) {
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

	TEST(CallsParserTest, CallsIdentWildTest) {
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

	TEST(CallsParserTest, CallsIdentIdentTest) {
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
	TEST(CallsParserTest, CallsTSynSynTest) {
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

	//Calls test
	TEST(CallsParserTest, semanticInvalidCallsProcVarTest) {
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

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(CallsParserTest, semanticInvalidCallsVarProcTest) {
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
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);

		EXPECT_TRUE(Utility::checkIsSemanticError(query));

	}

	TEST(CallsParserTest, semanticInvalidCallsProcStmtTest) {
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
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));

	}

	TEST(CallsParserTest, syntaticInvalidCallsEmptyQuotationTest) {

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

	TEST(CallsParserTest, syntaticInvalidCallsEmptyQuotationTest2) {
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

	TEST(CallsParserTest, semanticInvalidCallsUndeclaredSynTest) {
		QueryPatternRelRefParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(CallsParserTest, syntaticInvalidCallsUndeclaredSynTest2) {
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

		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(CallsParserTest, semanticInvalidCallsUndeclaredSynTest3) {
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
		validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(CallsParserTest, syntacticInvalidCallsProcIntTest) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);

	}
	TEST(CallsParserTest, syntaticInvalidCallsEmptyParamTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);

	}

	TEST(CallsParserTest, syntaticInvalidCallsEmptyParamTest2) {
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

	TEST(CallsParserTest, syntaticInvalidCallsEmptyParamTest3) {
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

	TEST(CallsParserTest, syntaticInvalidCallsEmptyParamTest4) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::CALLS, temp_token_chain), SyntacticErrorException);
	}

}