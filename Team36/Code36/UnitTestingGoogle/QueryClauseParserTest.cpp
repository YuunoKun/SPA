#include <iostream>

#include "pch.h"
#include "PQL/QueryPreprocessor/QueryClauseParser/QueryClauseParser.h"
#include "Common/Common.h"

namespace UnitTesting {
	class QueryClauseParserTest : public ::testing::Test {
	protected:
		QueryClauseParserTest() {
		}

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	// Test setStmtRef
	// 3 diff params
	TEST(QueryClauseParserTest, setStmtRefTest) {
		Query query;

		//Expected
		Synonym synonym;
		synonym.name = "s";
		Entity declared_stmt = Entity(EntityType::STMT, synonym);
		query.addEntity(declared_stmt);

		//Synonym
		//Result
		QueryToken stmt_s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		Entity ent_stmt = Utility::setStmtRef(query, stmt_s_token, EntityType::STMT);

		EXPECT_TRUE(ent_stmt == declared_stmt);

		/*============================================================================*/

		//WILDCARD
		//Result
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");
		Entity ent_wildacrd = Utility::setStmtRef(query, wildcard_token, EntityType::STMT);

		//Expected
		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		EXPECT_TRUE(ent_wildacrd == expected_ent_wildcard);

		/*============================================================================*/

		//CONSTANT
		//Result
		QueryToken constant_token = QueryToken(QueryToken::CONSTANT, "9");
		Entity ent_constant = Utility::setStmtRef(query, constant_token, EntityType::STMT);

		////Expected
		Entity expected_ent_constant = Entity(EntityType::STMT, "9");

		EXPECT_TRUE(ent_constant == expected_ent_constant);
	}

	// Test setEntRef
	TEST(QueryClauseParserTest, setEntRefTest) {
		QueryClauseParser validator;
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
		Entity ent_stmt = Utility::setEntRef(query, temp_token_chain, EntityType::VARIABLE);

		EXPECT_TRUE(ent_stmt == expected_declared_stmt);

		/*============================================================================*/
		temp_token_chain.clear();

		//WILDCARD
		//Result
		QueryToken wildcard_token = QueryToken(QueryToken::WILDCARD, "_");
		temp_token_chain.push_back(wildcard_token);
		Entity ent_wildacrd = Utility::setEntRef(query, temp_token_chain, EntityType::VARIABLE);

		//Expected
		Entity expected_ent_wildcard = Entity(EntityType::WILD);

		EXPECT_TRUE(ent_wildacrd == expected_ent_wildcard);

		/*============================================================================*/
		temp_token_chain.clear();

		//??IDENT ??
		//Result
		QueryToken quotation_open_token = QueryToken(QueryToken::QUOTATION_OPEN, "");
		QueryToken proc_name_token = QueryToken(QueryToken::IDENTIFIER, "procName");
		QueryToken quotation_close_token = QueryToken(QueryToken::QUOTATION_CLOSE, "");
		temp_token_chain.push_back(quotation_open_token);
		temp_token_chain.push_back(proc_name_token);
		temp_token_chain.push_back(quotation_close_token);

		Entity ent_constant = Utility::setEntRef(query, temp_token_chain, EntityType::VARIABLE);

		//Expected
		Entity expected_ent_constant = Entity(EntityType::VARIABLE, "procName");

		EXPECT_TRUE(ent_constant == expected_ent_constant);
	}

	// Test setExpr
	TEST(QueryClauseParserTest, setExprTest) {
		QueryClauseParser validator;

		std::vector<QueryToken> temp_token_chain;

		//Expected
		std::string expect_expr = " s";

		//Result
		QueryToken stmt_s_token = QueryToken(QueryToken::IDENTIFIER, "s");
		temp_token_chain.push_back(stmt_s_token);
		std::string result_expr = Utility::setExpr(temp_token_chain);

		EXPECT_TRUE(expect_expr == result_expr);
	}
}