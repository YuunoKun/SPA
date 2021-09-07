#include "pch.h"

#include "ModifiesSEvaluator.h"
#include "PKB.h"

namespace UnitTesting {

	class ModifiesSEvaluatorTest : public testing::Test {
	protected:
		ModifiesSEvaluatorTest() {
		}


		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		ModifiesSEvaluator evaluator;
	};
	TEST_F(ModifiesSEvaluatorTest, haveRelation) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, isRelation) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, haveRelationAtRight) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, haveRelationAtLeft) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, getRelations) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, getRightRelations) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, getLeftRelations) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, getRelationMatchLeft) {
		//TODO
	}

	TEST_F(ModifiesSEvaluatorTest, getRelationMatchRight) {
		//TODO
	}
}