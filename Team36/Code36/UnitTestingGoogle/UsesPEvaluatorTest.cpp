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
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, isRelation) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, haveRelationAtRight) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, haveRelationAtLeft) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, getRelations) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, getRightRelations) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, getLeftRelations) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, getRelationMatchLeft) {
		//TODO: Iteration 2
	}

	TEST_F(UsesPEvaluatorTest, getRelationMatchRight) {
		//TODO: Iteration 2
	}
}