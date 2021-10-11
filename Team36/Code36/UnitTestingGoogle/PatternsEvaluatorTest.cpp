#include "pch.h"

#include "RelationsEvaluator.h"
#include "pkb.h"
#include "PatternsEvaluator.h"
#include "ExprParser.h"

namespace UnitTesting {
	TEST(PatternsEvaluator, evaluateAssignPattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;
		ExprParser expr_parser;

		Entity assign = { ASSIGN, Synonym{"assign"} };
		Entity lhsSynonym = { VARIABLE, Synonym{"a"} };
		Entity lhsX = { VARIABLE, "x" };
		Entity lhsY = { VARIABLE, "y" };
		Entity lhsZ = { VARIABLE, "z" };
		Entity lhsN = { VARIABLE, "n" };

		std::vector<Entity> lhsNonExistList = { lhsSynonym, lhsX, lhsY, lhsZ, lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variableList = { "x", "y", "z", "y", "z", "z" };
		std::vector<std::string> expressionList = { "x", "y*x", "x+y", "z%x", "x-x", "x+y-z" };
		std::vector<stmt_index> stmtList = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variableList.size(); i++) {
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addVariable(variableList[i]);
			PKB::getInstance().addModifiesS(stmtList[i], variableList[i]);
			PKB::getInstance().addExprTree(stmtList[i], expr_parser.parse(expressionList[i]));
		}

		//Test for pattern a(x, _)
		std::vector<Entity> lhsExistList = { lhsSynonym, lhsX, lhsY, lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(assign, exist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhsNonExistList = { lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, "x")
		lhsExistList = { lhsSynonym, lhsX };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(assign, exist, "x", false);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhsNonExistList = { lhsY,  lhsZ, lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "x", false);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"x"_)
		lhsExistList = { lhsSynonym, lhsX, lhsY,  lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(assign, exist, "x", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhsNonExistList = { lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "x", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"y"_)
		lhsExistList = { lhsSynonym, lhsY,  lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(assign, exist, "y", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhsNonExistList = { lhsN, lhsX };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "y", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"z"_)
		lhsExistList = { lhsSynonym, lhsY,  lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(assign, exist, "z", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}
		lhsNonExistList = { lhsN, lhsX };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "z", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		//Test for pattern a(a, _"n"_)
		lhsNonExistList = { lhsSynonym, lhsX, lhsY, lhsZ, lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(assign, nonExist, "n", true);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}


	TEST(PatternsEvaluator, evaluateIfsPattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;

		Entity ifs = { IF, Synonym{"if"} };
		Entity lhsSynonym = { VARIABLE, Synonym{"a"} };
		Entity lhsX = { VARIABLE, "x" };
		Entity lhsY = { VARIABLE, "y" };
		Entity lhsZ = { VARIABLE, "z" };
		Entity lhsN = { VARIABLE, "n" };

		std::vector<Entity> lhsNonExistList = { lhsSynonym, lhsX, lhsY, lhsZ, lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(ifs, nonExist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variableList = { "x", "y", "z", "y", "z", "z" };
		std::vector<stmt_index> stmtList = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variableList.size(); i++) {
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addVariable(variableList[i]);
			PKB::getInstance().addIf(stmtList[i], variableList[i]);
		}

		std::vector<Entity> lhsExistList = { lhsSynonym, lhsX, lhsY, lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(ifs, exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}

		lhsNonExistList = { lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(ifs, nonExist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}

	TEST(PatternsEvaluator, evaluateWhilePattern) {
		PKB::getInstance().resetCache();
		PatternsEvaluator evaluator;

		Entity w = { WHILE, Synonym{"while"} };
		Entity lhsSynonym = { VARIABLE, Synonym{"a"} };
		Entity lhsX = { VARIABLE, "x" };
		Entity lhsY = { VARIABLE, "y" };
		Entity lhsZ = { VARIABLE, "z" };
		Entity lhsN = { VARIABLE, "n" };

		std::vector<Entity> lhsNonExistList = { lhsSynonym, lhsX, lhsY, lhsZ, lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(w, nonExist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}

		std::vector<var_name> variableList = { "x", "y", "z", "y", "z", "z" };
		std::vector<stmt_index> stmtList = { 1, 2, 3, 4, 5, 6 };

		for (unsigned int i = 0; i < variableList.size(); i++) {
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addVariable(variableList[i]);
			PKB::getInstance().addWhile(stmtList[i], variableList[i]);
		}

		std::vector<Entity> lhsExistList = { lhsSynonym, lhsX, lhsY, lhsZ };
		for (auto exist : lhsExistList) {
			QueryResult result;
			Pattern pattern(w, exist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_TRUE(result.haveResult());
		}

		lhsNonExistList = { lhsN };
		for (auto nonExist : lhsNonExistList) {
			QueryResult result;
			Pattern pattern(w, nonExist);
			evaluator.evaluatePattern(result, pattern);
			EXPECT_FALSE(result.haveResult());
		}
	}
}