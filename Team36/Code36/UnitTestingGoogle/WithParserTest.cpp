#include "pch.h"

#include "QueryClauseParser.h"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class WithParserTest : public ::testing::Test {
	protected:
		WithParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}

	};

	TEST(WithParserTest, WithIdentIdentTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::BOOLEAN, "var");

		Entity expected_2 = Entity(EntityType::BOOLEAN, "var2");

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var2" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithIdentSynProcStrTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::PROCEDURE, "procedure");
		expected_1.setAttribute(AttrRef::PROC_NAME);


		Synonym synonym;
		synonym.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procedure" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithIdentSynCallStrTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::CALL, "procedure");
		expected_1.setAttribute(AttrRef::PROC_NAME);


		Synonym synonym;
		synonym.name = "c";
		Entity expected_2 = Entity(EntityType::CALL, synonym);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "procedure" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithIdentSynVarStrTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::VARIABLE, "var");
		expected_1.setAttribute(AttrRef::VAR_NAME);


		Synonym synonym;
		synonym.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithIdentSynReadStrTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::READ, "read_var");
		expected_1.setAttribute(AttrRef::VAR_NAME);


		Synonym synonym;
		synonym.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "read_var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithIdentSynPrintStrTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::PRINT, "print_var");
		expected_1.setAttribute(AttrRef::VAR_NAME);


		Synonym synonym;
		synonym.name = "pr";
		Entity expected_2 = Entity(EntityType::PRINT, synonym);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "print_var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynPrintSynPrintStrTest) {
		Query query;
		//Expected

		Synonym synonym1;
		synonym1.name = "pr1";
		Entity expected_1 = Entity(EntityType::PRINT, synonym1);
		expected_1.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr2";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr2" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynPrintSynVarStrTest) {
		Query query;
		//Expected

		Synonym synonym1;
		synonym1.name = "pr";
		Entity expected_1 = Entity(EntityType::PRINT, synonym1);
		expected_1.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "var";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynProcSynPrintStrTest) {
		Query query;
		//Expected

		Synonym synonym1;
		synonym1.name = "proc";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym1);
		expected_1.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "proc" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynProcSynCallStrTest) {
		Query query;
		//Expected

		Synonym synonym1;
		synonym1.name = "proc";
		Entity expected_1 = Entity(EntityType::PROCEDURE, synonym1);
		expected_1.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CALL, synonym2);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "proc" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithConstConstTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::BOOLEAN, "9");

		Entity expected_2 = Entity(EntityType::BOOLEAN, "10");

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::CONSTANT, "10" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithConstSynProglineIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::PROG_LINE, "9");
		expected_1.setAttribute(AttrRef::STMT_INDEX);


		Synonym synonym;
		synonym.name = "progline";
		Entity expected_2 = Entity(EntityType::PROG_LINE, synonym);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithConstSynStmtIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::STMT, "9");
		expected_1.setAttribute(AttrRef::STMT_INDEX);


		Synonym synonym;
		synonym.name = "s";
		Entity expected_2 = Entity(EntityType::STMT, synonym);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "s" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithConstSynWhileIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::WHILE, "9");
		expected_1.setAttribute(AttrRef::STMT_INDEX);


		Synonym synonym;
		synonym.name = "w";
		Entity expected_2 = Entity(EntityType::WHILE, synonym);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "w" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithConstSynCallIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::CALL, "9");
		expected_1.setAttribute(AttrRef::STMT_INDEX);


		Synonym synonym;
		synonym.name = "c";
		Entity expected_2 = Entity(EntityType::CALL, synonym);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithConstSynReadIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::READ, "9");
		expected_1.setAttribute(AttrRef::STMT_INDEX);


		Synonym synonym;
		synonym.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithConstSynConstantIntTest) {
		Query query;
		//Expected
		Entity expected_1 = Entity(EntityType::CONSTANT, "9");
		expected_1.setAttribute(AttrRef::VALUE);


		Synonym synonym;
		synonym.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynProglineSynProglineIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "progline2";
		Entity expected_2 = Entity(EntityType::PROG_LINE, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline2" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynProglineSynIfIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynProglineSynCallIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "calls";
		Entity expected_2 = Entity(EntityType::CALL, synonym2);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "calls" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);



		expected_1.setAttribute(AttrRef::STMT_INDEX);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);
		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynProglineSynReadIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynProglineSynConstantTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynIfSynAssignIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "assign1";
		Entity expected_2 = Entity(EntityType::ASSIGN, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "assign1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynIfSynCallIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "ca";
		Entity expected_2 = Entity(EntityType::CALL, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ca" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynIfSynReadIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}
	TEST(WithParserTest, WithSynIfSynConstantIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynCallSynCallIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CALL, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "ca";
		Entity expected_2 = Entity(EntityType::CALL, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ca" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynCallSynReadIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CALL, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynCallSynConstantIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CALL, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "con";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "con" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynReadSynReadIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "re";
		Entity expected_1 = Entity(EntityType::READ, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "r";
		Entity expected_2 = Entity(EntityType::READ, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "re" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "r" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynReadSynConstantIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "re";
		Entity expected_1 = Entity(EntityType::READ, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "re" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	TEST(WithParserTest, WithSynConstantSynConstantIntTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "co";
		Entity expected_1 = Entity(EntityType::CONSTANT, synonym1);
		expected_1.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_1);


		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "co" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		validator.parseWith(query, temp_token_chain);

		EXPECT_TRUE(query.getClauses()[0].getRelation() == expected_rel);
	}

	//syntactic tests ----------------------------------------------------------------------------------------------------------------------------------
	TEST(WithParserTest, SyntacticInvalidCharTest) {
		Query query;

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::COMMA, "" });
		//temp_token_chain.push_back({ QueryToken::DOT, "" });
		//temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });

		EXPECT_THROW(validator.parseWith(query, temp_token_chain), SyntacticErrorException);
	}

	TEST(WithParserTest, SyntacInvalidAttrRefTest) {
		Query query;
		//Expected

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "var" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });

		EXPECT_THROW(validator.parseWith(query, temp_token_chain), SyntacticErrorException);
	}

	TEST(WithParserTest, SyntacInvalidAttrRefTest2) {
		Query query;
		//Expected

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });

		EXPECT_THROW(validator.parseWith(query, temp_token_chain), SyntacticErrorException);
	}

	TEST(WithParserTest, SyntacInvalidAttrRefTest3) {
		Query query;
		//Expected

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "5" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });

		EXPECT_THROW(validator.parseWith(query, temp_token_chain), SyntacticErrorException);
	}

	//semantic test ==========================================================================================================================================
	TEST(WithParserTest, SemanticIntStrTest) {
		Query query;

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		//temp_token_chain.push_back({ QueryToken::DOT, "" });
		//temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "iAmANumber" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });
		
		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest2) {
		Query query;

		Synonym synonym2;
		synonym2.name = "p";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		//RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		//temp_token_chain.push_back({ QueryToken::DOT, "" });
		//temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest3) {
		Query query;

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		//temp_token_chain.push_back({ QueryToken::DOT, "" });
		//temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest4) {
		Query query;
		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::CONSTANT, "9" });
		//temp_token_chain.push_back({ QueryToken::DOT, "" });
		//temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "print" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest5) {
		Query query;

		Synonym synonym2;
		synonym2.name = "p";
		Entity expected_2 = Entity(EntityType::PROG_LINE, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "iAmANumber" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest6) {
		Query query;

		Synonym synonym1;
		synonym1.name = "p";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest7) {
		Query query;

		Synonym synonym1;
		synonym1.name = "p";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest8) {
		Query query;

		Synonym synonym1;
		synonym1.name = "p";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "p" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest9) {
		Query query;

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "iAmANumber" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest10) {
		Query query;

		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest11) {
		Query query;

		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest12) {
		Query query;

		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest13) {
		Query query;

		Synonym synonym2;
		synonym2.name = "c";
		Entity expected_2 = Entity(EntityType::CONSTANT, synonym2);
		expected_2.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_OPEN, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "iAmANumber" });
		temp_token_chain.push_back({ QueryToken::QUOTATION_CLOSE, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest14) {
		Query query;

		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CONSTANT, synonym1);
		expected_1.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PROCEDURE, synonym2);
		expected_2.setAttribute(AttrRef::PROC_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;

		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });		
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest15) {
		Query query;

		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CONSTANT, synonym1);
		expected_1.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "v";
		Entity expected_2 = Entity(EntityType::VARIABLE, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticIntStrTest16) {
		Query query;

		Synonym synonym1;
		synonym1.name = "c";
		Entity expected_1 = Entity(EntityType::CONSTANT, synonym1);
		expected_1.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "pr";
		Entity expected_2 = Entity(EntityType::PRINT, synonym2);
		expected_2.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "c" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "pr" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest1) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		
		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest2) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest3) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest4) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "progline1";
		Entity expected_1 = Entity(EntityType::PROG_LINE, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "progline1" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VALUE, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest5) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "con";
		Entity expected_1 = Entity(EntityType::CONSTANT, synonym1);
		expected_1.setAttribute(AttrRef::VALUE);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "con" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest6) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "ifs";
		Entity expected_1 = Entity(EntityType::IF, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::VAR_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticAttrNameTest7) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "read";
		Entity expected_1 = Entity(EntityType::READ, synonym1);
		expected_1.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "read" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::PROC_NAME, "" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticSynReadTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "read";
		Entity expected_1 = Entity(EntityType::READ, synonym1);
		expected_1.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "read" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticSynCallTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "call";
		Entity expected_1 = Entity(EntityType::CALL, synonym1);
		expected_1.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "call" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}

	TEST(WithParserTest, SemanticSynVarTest) {
		Query query;
		//Expected
		Synonym synonym1;
		synonym1.name = "v";
		Entity expected_1 = Entity(EntityType::VARIABLE, synonym1);
		expected_1.setAttribute(AttrRef::VAR_NAME);
		query.addEntity(expected_1);

		Synonym synonym2;
		synonym2.name = "ifs";
		Entity expected_2 = Entity(EntityType::IF, synonym2);
		expected_2.setAttribute(AttrRef::STMT_INDEX);
		query.addEntity(expected_2);

		RelRef expected_rel = RelRef(RelType::WITH, expected_1, expected_2);

		//Result
		QueryClauseParser validator;
		std::vector<QueryToken> temp_token_chain;
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "v" });
		temp_token_chain.push_back({ QueryToken::EQUAL, "" });
		temp_token_chain.push_back({ QueryToken::IDENTIFIER, "ifs" });
		temp_token_chain.push_back({ QueryToken::DOT, "" });
		temp_token_chain.push_back({ QueryToken::STMT_INDEX, "" });

		validator.parseWith(query, temp_token_chain);
		EXPECT_TRUE(Utility::checkIsSemanticError(query));
	}
}