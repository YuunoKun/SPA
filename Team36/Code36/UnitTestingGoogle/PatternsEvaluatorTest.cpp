#include "pch.h"

#include "PKB/PKB.h"
#include "PQL/QueryEvaluator/PatternsEvaluator.h"
#include "Common/ExprParser.h"

namespace UnitTesting {
	TEST(PatternsEvaluator, evaluateAssignPattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;
		ExprParser expr_parser;

		Entity assign = { ASSIGN, Synonym{"assign"} };
		Entity lhs_synonym = { VARIABLE, Synonym{"a"} };
		Entity lhs_x = { VARIABLE, "x" };
		Entity lhs_y = { VARIABLE, "y" };
		Entity lhs_z = { VARIABLE, "z" };
		Entity lhs_n = { VARIABLE, "n" };

		std::vector<Entity> lhs_non_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z, lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variable_list = { "x", "y", "z", "y", "z", "z" };
		std::vector<std::string> expression_list = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
		std::vector<stmt_index> stmt_list = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variable_list.size(); i++) {
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addVariable(variable_list[i]);
			PKB::getInstance().addModifiesS(stmt_list[i], variable_list[i]);
			PKB::getInstance().addExprTree(stmt_list[i], expr_parser.parse(expression_list[i]));
		}

		//Test for pattern a(x, _)
		std::vector<Entity> lhs_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(assign, exist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhs_non_exist_list = { lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, "x")
		lhs_exist_list = { lhs_synonym, lhs_x };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(assign, exist, "x", false);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhs_non_exist_list = { lhs_y,  lhs_z, lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "x", false);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"x"_)
		lhs_exist_list = { lhs_synonym, lhs_x, lhs_y,  lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(assign, exist, "x", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhs_non_exist_list = { lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "x", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"y"_)
		lhs_exist_list = { lhs_synonym, lhs_y,  lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(assign, exist, "y", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhs_non_exist_list = { lhs_n, lhs_x };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "y", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"z"_)
		lhs_exist_list = { lhs_synonym, lhs_y,  lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(assign, exist, "z", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhs_non_exist_list = { lhs_n, lhs_x };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "z", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"n"_)
		lhs_non_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z, lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(assign, non_exist, "n", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}


	TEST(PatternsEvaluator, evaluateIfsPattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;

		Entity ifs = { IF, Synonym{"if"} };
		Entity lhs_synonym = { VARIABLE, Synonym{"a"} };
		Entity lhs_x = { VARIABLE, "x" };
		Entity lhs_y = { VARIABLE, "y" };
		Entity lhs_z = { VARIABLE, "z" };
		Entity lhs_n = { VARIABLE, "n" };

		std::vector<Entity> lhs_non_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z, lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(ifs, non_exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variable_list = { "x", "y", "z", "y", "z", "z" };
		std::vector<stmt_index> stmtList = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variable_list.size(); i++) {
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addVariable(variable_list[i]);
			PKB::getInstance().addIf(stmtList[i], variable_list[i]);
		}

		std::vector<Entity> lhs_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(ifs, exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}

		lhs_non_exist_list = { lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(ifs, non_exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}

	TEST(PatternsEvaluator, evaluateWhilePattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;

		Entity w = { WHILE, Synonym{"while"} };
		Entity lhs_synonym = { VARIABLE, Synonym{"a"} };
		Entity lhs_x = { VARIABLE, "x" };
		Entity lhs_y = { VARIABLE, "y" };
		Entity lhs_z = { VARIABLE, "z" };
		Entity lhs_n = { VARIABLE, "n" };

		std::vector<Entity> lhs_non_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z, lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(w, non_exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variable_list = { "x", "y", "z", "y", "z", "z" };
		std::vector<stmt_index> stmtList = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variable_list.size(); i++) {
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addVariable(variable_list[i]);
			PKB::getInstance().addWhile(stmtList[i], variable_list[i]);
		}

		std::vector<Entity> lhs_exist_list = { lhs_synonym, lhs_x, lhs_y, lhs_z };
		for (auto exist : lhs_exist_list) {
			QueryResult result;
			Pattern pattern(w, exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}

		lhs_non_exist_list = { lhs_n };
		for (auto non_exist : lhs_non_exist_list) {
			QueryResult result;
			Pattern pattern(w, non_exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}
}