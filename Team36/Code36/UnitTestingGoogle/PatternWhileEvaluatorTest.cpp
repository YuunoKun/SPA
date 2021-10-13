#include "pch.h"

#include "pkb.h"
#include "PatternWhileEvaluator.h"
#include "ExprParser.h"
#include "PKBAdapter.h"

namespace UnitTesting {
	class PatternWhileEvaluatorTest : public testing::Test {
	protected:
		PatternWhileEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		PatternWhileEvaluator evaluator;
	};

	TEST_F(PatternWhileEvaluatorTest, evaluateSynonym) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addVariable(z);
		PKB::getInstance().addWhile(1, x);
		PKB::getInstance().addWhile(3, y);

		std::vector<std::pair<stmt_index, var_name>> v = pkb.getAllWhileUses();
		Entity left = { WHILE, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonym(Pattern(left, right)), t);

	}

	TEST_F(PatternWhileEvaluatorTest, evaluateConstant) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addVariable(z);
		PKB::getInstance().addWhile(1, x);
		PKB::getInstance().addWhile(3, y);

		std::vector<stmt_index> v = { 1 };
		Entity left = { WHILE, Synonym{"a"} };
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

	TEST_F(PatternWhileEvaluatorTest, evaluateWild) {
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addVariable(z);
		PKB::getInstance().addWhile(1, x);
		PKB::getInstance().addWhile(3, y);

		std::vector<stmt_index> v = { 1, 3 };
		Entity left = { WHILE, Synonym{"a"} };
		ResultTable t(left, v);
		EXPECT_EQ(evaluator.evaluateWild(Pattern(left, { WILD })), t);
	}
}