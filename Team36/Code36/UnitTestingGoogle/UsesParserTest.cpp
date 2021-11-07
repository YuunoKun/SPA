#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class UsesParserTest : public ::testing::Test {
	protected:
		UsesParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	//USES_S
	TEST(UsesParserTest, UsesSSynSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);


		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::STMT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_S, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
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
	TEST(UsesParserTest, UsesPSynSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(UsesParserTest, UsesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::USES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
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

	// Invalid tests -------------------------------------------------------------------------------------------------------------------------------	

	// UsesS, UsesP Test
	// test semantically invalid Uses with 1st param as WILDCARD
	TEST(UsesParserTest, semanticInvalidUsesSWildSynTest) {
		QueryClauseParser validator;

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

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(UsesParserTest, semanticInvalidUsesSWildWildTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	//syntactic error(integer 2nd param) and semantic error(wildcard 1st param) here
	TEST(UsesParserTest, syntacticInvalidUsesSWildIntTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(UsesParserTest, semanticInvalidUsesSWildIdentTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::USES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}
}