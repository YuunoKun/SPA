#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class AffectsParserTest : public ::testing::Test {
	protected:
		AffectsParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	// AFFECT
	TEST(AffectsParserTest, AffectsSynSynTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_1);

		Synonym synonym2;
		synonym2.name = "s1";
		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "3");

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "3");

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsParserTest, AffectsIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "4");

		RelRef expected_rel = RelRef(RelType::AFFECT, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//Invalid Test -------------------------------------------------------------------------------------------------------------------
	TEST(AffectsParserTest, syntacticInvalidAffectsIdentWildTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ident" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });


		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain), SyntacticErrorException);
	}

	TEST(AffectsParserTest, semanticInvalidAffectsVarWildTest) {
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

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsParserTest, semanticInvalidAffectsUndeclaredSynTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsParserTest, semanticInvalidAffectsUndeclaredSynTest2) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "x";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsParserTest, semanticInvalidAffectsUndeclaredSynTest3) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "y";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsParserTest, syntaticInvalidAffectsEmptyParamTest) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain), SyntacticErrorException);

	}

	TEST(AffectsParserTest, syntaticInvalidAffectsEmptyParamTest2) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain), SyntacticErrorException);
	}

	TEST(AffectsParserTest, syntaticInvalidAffectsEmptyParamTest3) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain), SyntacticErrorException);

	}

	TEST(AffectsParserTest, syntaticInvalidAffectsEmptyParamTest4) {
		QueryClauseParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS, temp_token_chain), SyntacticErrorException);
	}

}