#include <iostream> 

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class NextTParserTest : public ::testing::Test {
	protected:
		NextTParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	TEST(NextTParserTest, NextTSynSynTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "a";
		Entity expected_2 = Entity(EntityType::ASSIGN, synonym2);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTSynWildTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "3");

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "3");

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextTParserTest, NextTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "4");

		RelRef expected_rel = RelRef(RelType::NEXT_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//Invalid

	TEST(NextTParserTest, syntacticInvalidNextTIdentWildTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ident" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain), SyntacticErrorException);
	}

	TEST(NextTParserTest, semanticInvalidNextTVarWildTest) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}
}