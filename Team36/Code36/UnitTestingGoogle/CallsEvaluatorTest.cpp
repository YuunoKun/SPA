#include "pch.h"

#include "CallsEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class CallsEvaluatorTest : public testing::Test {
	protected:
		CallsEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		CallsEvaluator evaluator;
	};
	TEST_F(CallsEvaluatorTest, haveRelation) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);

		EXPECT_FALSE(evaluator.haveRelation());

		PKB::getInstance().addCallsP(main1, main2);

		EXPECT_TRUE(evaluator.haveRelation());
	}

	TEST_F(CallsEvaluatorTest, isRelation) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		EXPECT_TRUE(evaluator.isRelation(e1, e2));
		EXPECT_TRUE(evaluator.isRelation(e2, e3));
		EXPECT_FALSE(evaluator.isRelation(e1, e1));
		EXPECT_FALSE(evaluator.isRelation(e1, e3));
		EXPECT_FALSE(evaluator.isRelation(e1, e4));
		EXPECT_FALSE(evaluator.isRelation(e2, e1));
		EXPECT_FALSE(evaluator.isRelation(e2, e2));
		EXPECT_FALSE(evaluator.isRelation(e2, e4));
		EXPECT_FALSE(evaluator.isRelation(e3, e1));
		EXPECT_FALSE(evaluator.isRelation(e3, e2));
		EXPECT_FALSE(evaluator.isRelation(e3, e3));
		EXPECT_FALSE(evaluator.isRelation(e3, e4));
		EXPECT_FALSE(evaluator.isRelation(e4, e1));
		EXPECT_FALSE(evaluator.isRelation(e4, e2));
		EXPECT_FALSE(evaluator.isRelation(e4, e3));
		EXPECT_FALSE(evaluator.isRelation(e4, e4));
	}

	TEST_F(CallsEvaluatorTest, haveRelationAtRight) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		EXPECT_TRUE(evaluator.haveRelationAtRight(e1));
		EXPECT_TRUE(evaluator.haveRelationAtRight(e2));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e3));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e4));
	}

	TEST_F(CallsEvaluatorTest, haveRelationAtLeft) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		EXPECT_FALSE(evaluator.haveRelationAtLeft(e1));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e2));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e3));
		EXPECT_FALSE(evaluator.haveRelationAtLeft(e4));
	}

	TEST_F(CallsEvaluatorTest, getRelations) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		std::vector<std::pair<proc_name, var_name>> v = pkb.getCallsPRelation();
		Entity left = { PROCEDURE, Synonym{"a"} };
		Entity right = { PROCEDURE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);
	}

	TEST_F(CallsEvaluatorTest, getRightRelations) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		std::vector<std::string> v = pkb.getCalleeP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
	}

	TEST_F(CallsEvaluatorTest, getLeftRelations) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		std::vector<std::string> v = pkb.getCallerP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
	}

	TEST_F(CallsEvaluatorTest, getRelationMatchLeft) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);

		std::vector<proc_name> v = { main2 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = e1;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { main3 };
		match = e2;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		match = e3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		match = e4;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
	}

	TEST_F(CallsEvaluatorTest, getRelationMatchRight) {
		proc_name main1 = "main1";
		proc_name main2 = "main2";
		proc_name main3 = "main3";
		proc_name main4 = "main4";
		Entity e1 = { PROCEDURE, main1 };
		Entity e2 = { PROCEDURE, main2 };
		Entity e3 = { PROCEDURE, main3 };
		Entity e4 = { PROCEDURE, main4 };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(main3);
		PKB::getInstance().addProcedure(main4);
		PKB::getInstance().addCallsP(main1, main2);
		PKB::getInstance().addCallsP(main2, main3);


		std::vector<proc_name> v = { main1 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = e2;
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { main2 };
		match = e3;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { };
		match = e1;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { };
		match = e4;
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

	}
}
