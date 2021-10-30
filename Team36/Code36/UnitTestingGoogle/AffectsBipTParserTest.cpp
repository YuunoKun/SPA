#include "pch.h"

#include "QueryPatternRelRefParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class AffectsBipTParserTest : public ::testing::Test {
	protected:
		AffectsBipTParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	// AFFECT_BIP_T
	TEST(AffectsBipTParserTest, AffectsBipTSynSynTest) {
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

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s1" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTSynIntTest) {
		Query query;

		//Synonym
		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "3");

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTWildSynTest) {
		Query query;

		//Synonym
		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTWildWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTWildIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::WILD);

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "3");

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, synonym);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(AffectsBipTParserTest, AffectsBipTIntIntTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::ASSIGN, "3");

		Entity expected_stmt_2 = Entity(EntityType::ASSIGN, "4");

		RelRef expected_rel = RelRef(RelType::AFFECT_BIP_T, expected_stmt_1, expected_stmt_2);

		//Result
		QueryPatternRelRefParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "3" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "4" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//Invalid Test -------------------------------------------------------------------------------------------------------------------
	TEST(AffectsBipTParserTest, syntacticInvalidAffectsBipTIdentWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ident" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });


		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain), SyntacticErrorException);
	}

	TEST(AffectsBipTParserTest, semanticInvalidAffectsBipTStmtWildTest) {
		QueryPatternRelRefParser validator;

		Query query;
		Synonym synonym;
		synonym.name = "v";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}


	TEST(AffectsBipTParserTest, semanticInvalidAffectsBipTVarWildTest) {
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

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsBipTParserTest, semanticInvalidAffectsBipTUndeclaredSynTest) {
		QueryPatternRelRefParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });
		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsBipTParserTest, semanticInvalidAffectsBipTUndeclaredSynTest2) {
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

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsBipTParserTest, semanticInvalidAffectsBipTUndeclaredSynTest3) {
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

		validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(AffectsBipTParserTest, syntaticInvalidAffectsBipTEmptyParamTest) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain), SyntacticErrorException);

	}

	TEST(AffectsBipTParserTest, syntaticInvalidAffectsBipTEmptyParamTest2) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain), SyntacticErrorException);
	}

	TEST(AffectsBipTParserTest, syntaticInvalidAffectsBipTEmptyParamTest3) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain), SyntacticErrorException);

	}

	TEST(AffectsBipTParserTest, syntaticInvalidAffectsBipTEmptyParamTest4) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::AFFECTS_BIP_T, temp_token_chain), SyntacticErrorException);
	}

}