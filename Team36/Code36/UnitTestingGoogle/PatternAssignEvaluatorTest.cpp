#include "pch.h"

#include "pkb.h"
#include "PatternAssignEvaluator.h"
#include "ExprParser.h"
#include "PKBAdapter.h"

namespace UnitTesting {
	class PatternAssignEvaluatorTest : public testing::Test {
	protected:
		PatternAssignEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addVariable(y);
			PKB::getInstance().addVariable(z);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addModifiesS(3, y);
			PKB::getInstance().addExprTree(1, expr_parser.parse(x));
			PKB::getInstance().addExprTree(3, expr_parser.parse(y));
		}

		PKBAdapter pkb;
		PatternAssignEvaluator evaluator;
		ExprParser expr_parser;


		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		Entity v1 = { VARIABLE, x };
		Entity v2 = { VARIABLE, y };
		Entity v3 = { VARIABLE, z };
	};

	TEST_F(PatternAssignEvaluatorTest, evaluateSynonym) {
		std::vector<std::pair<stmt_index, var_name>> v = pkb.getAssignInfo();
		Entity left = { ASSIGN, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonym(Pattern(left, right, "", true)), t);
		std::vector<std::pair<stmt_index, var_name>> v1 = { v[0] };
		t = ResultTable(header, v1);
		EXPECT_EQ(evaluator.evaluateSynonym(Pattern(left, right, x, true)), t);
		std::vector<std::pair<stmt_index, var_name>> v2 = { v[1] };
		t = ResultTable(header, v2);
		EXPECT_EQ(evaluator.evaluateSynonym(Pattern(left, right, y, true)), t);

	}

	TEST_F(PatternAssignEvaluatorTest, evaluateConstant) {
		std::vector<stmt_index> v = { 1 };
		Entity left = { ASSIGN, Synonym{"a"} };
		Entity match = v1;
		ResultTable t(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match, "", true)), t);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match, x, true)), t);

		v = { 3 };
		match = v2;
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match, "", true)), t);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match, y, true)), t);

		v = { };
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, v3, "", true)), t);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, v1, y, true)), t);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, v2, x, true)), t);
	}

	TEST_F(PatternAssignEvaluatorTest, evaluateWild) {
		std::vector<stmt_index> v = { 1, 3 };
		Entity left = { ASSIGN, Synonym{"a"} };
		ResultTable t(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD }, "", true)), t);

		v = { 1 };
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD }, x, true)), t);

		v = { 3 };
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD }, y, true)), t);

		v = { };
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD }, z, true)), t);
	}
}