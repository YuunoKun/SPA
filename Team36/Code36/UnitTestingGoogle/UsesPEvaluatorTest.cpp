#include "pch.h"

#include "UsesPEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class UsesPEvaluatorTest : public testing::Test {
	protected:
		UsesPEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addProcedure(main1);
			PKB::getInstance().addProcedure(main2);
			PKB::getInstance().addProcedure(sub1);
			PKB::getInstance().addProcedure(sub2);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addUsesP(main1, x);
			PKB::getInstance().addVariable(y);
			PKB::getInstance().addUsesP(main2, y);
		}

		PKBAdapter pkb;
		UsesPEvaluator evaluator;

		proc_name main1 = "main1";
		proc_name sub1 = "sub1";
		proc_name main2 = "main2";
		proc_name sub2 = "sub2";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, sub1 };
		Entity e3 = { PROCEDURE, main2 };
		Entity e4 = { PROCEDURE, sub2 };
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		Entity v1 = { VARIABLE, x };
		Entity v2 = { VARIABLE, y };
		Entity v3 = { VARIABLE, z };
	};

	TEST_F(UsesPEvaluatorTest, evaluateWildAndWild) {
		EXPECT_THROW(evaluator.evaluateWildAndWild(), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, evaluateConstantAndConstant) {
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, v1));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e3, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v3));
	}

	TEST_F(UsesPEvaluatorTest, evaluateConstantAndWild) {
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e2));
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e4));
	}

	TEST_F(UsesPEvaluatorTest, evaluateWildAndConstant) {
		Entity v1 = { VARIABLE, "v"};
		EXPECT_THROW(evaluator.evaluateWildAndConstant(v1), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, evaluateSynonymAndSynonym) {
		std::vector<std::pair<proc_name, var_name>> v = pkb.getUsesPRelation();
		Entity left = { PROCEDURE, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(UsesPEvaluatorTest, evaluateWildAndSynonym) {
		Entity header = { VARIABLE, Synonym{"a"} };
		EXPECT_THROW(evaluator.evaluateWildAndSynonym(header), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, evaluateSynonymAndWild) {
		std::vector<proc_name> v = pkb.getUsesP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(UsesPEvaluatorTest, evaluateConstantAndSynonym) {
		std::vector<var_name> v = { x };
		Entity header = { VARIABLE, Synonym{"a"} };
		Entity match = e1;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { y };
		match = e3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		match = e2;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		match = e4;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
	}

	TEST_F(UsesPEvaluatorTest, evaluateSynonymAndConstant) {
		std::vector<proc_name> v = { main2 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = v2;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { main1 };
		match = v1;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = {};
		match = v3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
	}
}