#include "pch.h"

#include "pkb.h"
#include "PatternIfEvaluator.h"
#include "ExprParser.h"
#include "PKBAdapter.h"

namespace UnitTesting {
	class PatternIfEvaluatorTest : public testing::Test {
	protected:
		PatternIfEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addVariable(y);
			PKB::getInstance().addVariable(z);
			PKB::getInstance().addIf(1, x);
			PKB::getInstance().addIf(3, y);
		}

		PKBAdapter pkb;
		PatternIfEvaluator evaluator;

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		Entity v1 = { VARIABLE, x };
		Entity v2 = { VARIABLE, y };
		Entity v3 = { VARIABLE, z };
	};

	TEST_F(PatternIfEvaluatorTest, evaluateSynonym) {
		std::vector<std::pair<stmt_index, var_name>> v = pkb.getAllIfUses();
		Entity left = { IF, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonym(Pattern(left, right)), t);

	}

	TEST_F(PatternIfEvaluatorTest, evaluateConstant) {
		std::vector<stmt_index> v = { 1 };
		Entity left = { IF, Synonym{"a"} };
		Entity match = v1;
		ResultTable t(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match)), t);

		v = { 3 };
		match = v2;
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match)), t);

		v = { };
		match = v3;
		t = ResultTable(left, v);
		EXPECT_EQ(evaluator.evaluateConstant(Pattern(left, match)), t);
	}

	TEST_F(PatternIfEvaluatorTest, evaluateWild) {
		std::vector<stmt_index> v = { 1, 3 };
		Entity left = { IF, Synonym{"a"} };
		ResultTable t(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD })), t);
	}
}