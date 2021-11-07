#include "pch.h"

#include "PQL/QueryEvaluator/RelationEvaluator/CallsEvaluator.h"
#include "PKB/PKB.h"

namespace UnitTesting {
	class CallsEvaluatorTest : public testing::Test {
	protected:
		CallsEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
			PKB::getInstance().addProcedure(main1);
			PKB::getInstance().addProcedure(main2);
			PKB::getInstance().addProcedure(main3);
			PKB::getInstance().addProcedure(main4);
			PKB::getInstance().addCallsP(main1, main2);
			PKB::getInstance().addCallsP(main2, main3);
		}

		PKBAdapter pkb;
		CallsEvaluator evaluator;

		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

	};
	TEST_F(CallsEvaluatorTest, evaluateWildAndWild) {
		PKB::getInstance().resetCache();

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);

		EXPECT_FALSE(evaluator.evaluateWildAndWild());

		PKB::getInstance().addCallsP(main1, main2);

		EXPECT_TRUE(evaluator.evaluateWildAndWild());
	}

	TEST_F(CallsEvaluatorTest, evaluateConstantAndConstant) {

		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, e2));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e2, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e4));
	}

	TEST_F(CallsEvaluatorTest, evaluateConstantAndWild) {

		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e1));
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e4));
	}

	TEST_F(CallsEvaluatorTest, evaluateWildAndConstant) {

		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e1));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e2));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e3));
		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e4));
	}

	TEST_F(CallsEvaluatorTest, evaluateSynonymAndSynonym) {

		std::vector<std::pair<proc_name, var_name>> v = pkb.getCallsPRelation();
		Entity left = { PROCEDURE, Synonym{"a"} };
		Entity right = { PROCEDURE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(CallsEvaluatorTest, evaluateWildAndSynonym) {

		std::vector<std::string> v = pkb.getCalleeP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);
	}

	TEST_F(CallsEvaluatorTest, evaluateSynonymAndWild) {

		std::vector<std::string> v = pkb.getCallerP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(CallsEvaluatorTest, evaluateConstantAndSynonym) {

		std::vector<proc_name> v = { main2 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = e1;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { main3 };
		match = e2;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		match = e3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		match = e4;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
	}

	TEST_F(CallsEvaluatorTest, evaluateSynonymAndConstant) {

		std::vector<proc_name> v = { main1 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = e2;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { main2 };
		match = e3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { };
		match = e1;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { };
		match = e4;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

	}
}
