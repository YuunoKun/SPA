#include "pch.h"

#include "UsesPEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class UsesPEvaluatorTest : public testing::Test {
	protected:
		UsesPEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		UsesPEvaluator evaluator;
	};

	TEST_F(UsesPEvaluatorTest, haveRelation) {
		proc_name main1 = "main1";
		var_name x = "x";
		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addVariable(x);

		EXPECT_THROW(evaluator.haveRelation(), std::invalid_argument);

		PKB::getInstance().addUsesP(main1, x);
		EXPECT_THROW(evaluator.haveRelation(), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, isRelation) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		EXPECT_TRUE(evaluator.isRelation(e1, v1));
		EXPECT_FALSE(evaluator.isRelation(e1, v2));
		EXPECT_FALSE(evaluator.isRelation(e1, v3));
		EXPECT_FALSE(evaluator.isRelation(e2, v1));
		EXPECT_FALSE(evaluator.isRelation(e2, v2));
		EXPECT_FALSE(evaluator.isRelation(e2, v3));
		EXPECT_FALSE(evaluator.isRelation(e3, v1));
		EXPECT_TRUE(evaluator.isRelation(e3, v2));
		EXPECT_FALSE(evaluator.isRelation(e3, v3));
		EXPECT_FALSE(evaluator.isRelation(e4, v1));
		EXPECT_FALSE(evaluator.isRelation(e4, v2));
		EXPECT_FALSE(evaluator.isRelation(e4, v3));
	}

	TEST_F(UsesPEvaluatorTest, haveRelationAtRight) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		EXPECT_TRUE(evaluator.haveRelationAtRight(e1));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e2));
		EXPECT_TRUE(evaluator.haveRelationAtRight(e3));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e4));
	}

	TEST_F(UsesPEvaluatorTest, haveRelationAtLeft) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		EXPECT_THROW(evaluator.haveRelationAtLeft(v1), std::invalid_argument);
		EXPECT_THROW(evaluator.haveRelationAtLeft(v2), std::invalid_argument);
		EXPECT_THROW(evaluator.haveRelationAtLeft(v3), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, getRelations) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		std::vector<std::pair<proc_name, var_name>> v = pkb.getUsesPRelation();
		Entity left = { PROCEDURE, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);
	}

	TEST_F(UsesPEvaluatorTest, getRightRelations) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		Entity header = { VARIABLE, Synonym{"a"} };
		EXPECT_THROW(evaluator.getRightRelations(header), std::invalid_argument);
	}

	TEST_F(UsesPEvaluatorTest, getLeftRelations) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		std::vector<proc_name> v = pkb.getUsesP();
		Entity header = { PROCEDURE, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
	}

	TEST_F(UsesPEvaluatorTest, getRelationMatchLeft) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		std::vector<var_name> v = { x };
		Entity header = { VARIABLE, Synonym{"a"} };
		Entity match = { PROCEDURE, main1 };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { y };
		match = { PROCEDURE, main2 };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		match = { PROCEDURE, sub1 };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		match = { PROCEDURE, sub2 };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
	}

	TEST_F(UsesPEvaluatorTest, getRelationMatchRight) {
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
		Entity v1 = { VARIABLE, "x" };
		Entity v2 = { VARIABLE, "y" };
		Entity v3 = { VARIABLE, "z" };

		PKB::getInstance().addProcedure(main1);
		PKB::getInstance().addProcedure(main2);
		PKB::getInstance().addProcedure(sub1);
		PKB::getInstance().addProcedure(sub2);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(main1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesP(main2, y);

		std::vector<proc_name> v = { main2 };
		Entity header = { PROCEDURE, Synonym{"a"} };
		Entity match = { VARIABLE, y };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { main1 };
		match = { VARIABLE, x };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = {};
		match = { VARIABLE, z };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
	}
}