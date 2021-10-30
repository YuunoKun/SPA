#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class NextBipTParserTest : public ::testing::Test {
	protected:
		NextBipTParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	TEST(NextBipTParserTest, NextBipTSynSynTest) {
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

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_1, expected_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "a" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTSynWildTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "3");

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "3");

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(NextBipTParserTest, NextBipTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::PROG_LINE, "3");

		Entity expected_stmt_2 = Entity(EntityType::PROG_LINE, "4");

		RelRef expected_rel = RelRef(RelType::NEXT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//Invalid

	TEST(NextBipTParserTest, syntacticInvalidNextBipTIdentWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ident" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain), SyntacticErrorException);
	}

	TEST(NextBipTParserTest, semanticInvalidNextBipTVarWildTest) {
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
		validator.parseParameterSuchThat(query, QueryToken::NEXT_BIP_T, temp_token_chain);

		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}
}