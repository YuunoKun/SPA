#include "pch.h"

#include "ModifiesPEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class ModifiesPEvaluatorTest : public testing::Test {
	protected:
		ModifiesPEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		ModifiesPEvaluator evaluator;
	};
	TEST_F(ModifiesPEvaluatorTest, haveRelation) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, isRelation) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, haveRelationAtRight) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, haveRelationAtLeft) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, getRelations) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, getRightRelations) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, getLeftRelations) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, getRelationMatchLeft) {
		//TODO: Iteration 2
	}

	TEST_F(ModifiesPEvaluatorTest, getRelationMatchRight) {
		//TODO: Iteration 2
	}
}