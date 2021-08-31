#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {
	TEST(QueryEvaluator, evaluateQueryVariable) {
		//PKB::getInstance().setStmts({ {STMT_PRINT,1 }, {STMT_PRINT,2}, {STMT_PRINT,3} });
		PKB::getInstance().setVariables({ "a", "b", "c" });

		Query q;
		Entity e = { VARIABLE, "1" };
		q.addEntity(e);
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ("a, b, c", qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryConstant) {
		PKB::getInstance().setConstants({ 1, 2, 3 });

		Query q;
		Entity e = { CONSTANT, "1" };
		q.addEntity(e);
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ("1, 2, 3", qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryProcedures) {
		PKB::getInstance().setProcedures({ "x", "y", "z" });

		Query q;
		Entity e = { PROCEDURE, "1" };
		q.addEntity(e);
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ("x, y, z", qe.evaluateQuery(q));
	}
}