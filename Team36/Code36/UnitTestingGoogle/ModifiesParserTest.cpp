#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class ModifiesParserTest : public ::testing::Test {
	protected:
		ModifiesParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	//MODIFIES_S

	TEST(ModifiesParserTest, ModifiesSSynSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesSSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_stmt_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_stmt_1);

		Entity expected_stmt_2 = Entity(EntityType::WILD);


		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesSSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity expected_1 = Entity(EntityType::STMT, synonym);
		query.addEntity(expected_1);


		Entity expected_2 = Entity(EntityType::VARIABLE, "program");


		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesSIntSynTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "5");

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_stmt_2 = Entity(EntityType::VARIABLE, synonym2);
		query.addEntity(expected_stmt_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesSIntWildTest) {
		Query query;

		//Expected
		Entity expected_stmt_1 = Entity(EntityType::STMT, "5");

		Entity expected_stmt_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_stmt_1, expected_stmt_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesSIntIdentTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::STMT, "5");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_S, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
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
	TEST(ModifiesParserTest, ModifiesPSynSynTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesPSynWildTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesPSynIdentTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "p";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym);
		query.addEntity(expected_1);

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesPIdentSynTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesPIdentWildTest) {
		Query query;

		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::WILD);

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "main" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, ModifiesPIdentIdentTest) {
		Query query;

		//Expected

		Entity expected_1 = Entity(EntityType::PROCEDURE, "main");

		Entity expected_2 = Entity(EntityType::VARIABLE, "var");

		RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

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
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesSWildSynTest) {
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

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesSWildWildTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	// is both semnatically invalid (1st param wildcard) and syntactically invalid (integer for 2nd param)
	//syntactic takes precedence
	TEST(ModifiesParserTest, syntacticInvalidModifiesSWildIntTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesSWildIdentTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::WILDCARD, "" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "program" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}


	TEST(ModifiesParserTest, semanticInvalidModifiesSVarVarTest) {
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
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v1" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesPProcProcTest) {
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

		//RelRef expected_rel = RelRef(RelType::MODIFIES_P, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p1" });

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, syntacticInvalidModifiesSIntIntTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });


		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesSEmptyQuotationTest) {
		QueryClauseParser validator;

		Query query;

		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "1" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesSEmptyQuotationTest2) {
		QueryClauseParser validator;

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

	// Should be handled by Query Tokenizer

		TEST(ModifiesParserTest, syntaticInvalidModifiesSMissingQuotationTest3) {
		QueryClauseParser validator;

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
	TEST(ModifiesParserTest, syntaticInvalidModifiesSMissingQuotationTest4) {
		QueryClauseParser validator;

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

	TEST(ModifiesParserTest, semanticInvalidModifiesSUndeclaredSynTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "x" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "y" });
		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesSUndeclaredSynTest2) {
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

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, semanticInvalidModifiesUndeclaredSynTest3) {
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

		validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesEmptyParamTest) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		//temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesEmptyParamTest2) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesEmptyParamTest3) {
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

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);

	}

	TEST(ModifiesParserTest, syntaticInvalidModifiesEmptyParamTest4) {
		QueryClauseParser validator;

		Query query;
		std::vector<QueryToken> temp_token_chain;
		//temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::WILDCARD, "" });

		EXPECT_THROW(validator.parseParameterSuchThat(query, QueryToken::MODIFIES_S, temp_token_chain), SyntacticErrorException);
	}

}