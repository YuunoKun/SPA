#include "pch.h"

#include "UsesSEvaluator.h"
#include "PKB.h"

namespace UnitTesting {

	class UsesSEvaluatorTest : public testing::Test {
	protected:
		UsesSEvaluatorTest() {
		}


		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		UsesSEvaluator evaluator;
	};

	TEST_F(UsesSEvaluatorTest, haveRelation) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, isRelation) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, haveRelationAtRight) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, haveRelationAtLeft) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, getRelations) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, getRightRelations) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, getLeftRelations) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, getRelationMatchLeft) {
		//Todo
	}

	TEST_F(UsesSEvaluatorTest, getRelationMatchRight) {
		//Todo
	}
}