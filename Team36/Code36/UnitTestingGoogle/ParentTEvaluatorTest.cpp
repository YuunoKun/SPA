#include "pch.h"

#include "ParentTEvaluator.h"
#include "PKB.h"

namespace UnitTesting {

	class ParentTEvaluatorTest : public testing::Test {
	protected:
		ParentTEvaluatorTest() {
		}


		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		ParentTEvaluator evaluator;
	};

	TEST_F(ParentTEvaluatorTest, haveRelation) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, isRelation) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, haveRelationAtRight) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, haveRelationAtLeft) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, getRelations) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, getRightRelations) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, getLeftRelations) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, getRelationMatchLeft) {
		//Todo
	}

	TEST_F(ParentTEvaluatorTest, getRelationMatchRight) {
		//Todo
	}
}