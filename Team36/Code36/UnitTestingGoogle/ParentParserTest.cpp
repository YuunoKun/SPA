#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class ParentParserTest : public ::testing::Test {
	protected:
		ParentParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	// PARENT
	TEST(ParentParserTest, ParentSynSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentSynWildTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::STMT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentWildSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::STMT, "3");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ParentParserTest, ParentIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "3");

		Entity expected_stmt_2 = Entity(EntityType::STMT, "4");

		RelRef expected_rel = RelRef(RelType::PARENT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::PARENT, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
}