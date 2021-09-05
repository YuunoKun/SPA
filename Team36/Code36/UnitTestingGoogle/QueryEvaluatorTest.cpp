#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {
	TEST(QueryEvaluator, evaluateQueryVariable) {
		//PKB::getInstance().setStmts({ {STMT_PRINT,1 }, {STMT_PRINT,2}, {STMT_PRINT,3} });
		PKB::getInstance().resetCache();
		PKB::getInstance().setVariables({ "a", "b", "c" });

		std::list<std::string> result = { "a", "b", "c" };

		Query q;
		Entity e = { VARIABLE, "1" };
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryConstant) {
		PKB::getInstance().resetCache();
		PKB::getInstance().setConstants({ 1, 2, 3 });

		std::list<std::string> result = { "1", "2", "3" };

		Query q;
		Entity e = { CONSTANT, "1" };
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryProcedures) {
		PKB::getInstance().resetCache();
		PKB::getInstance().setProcedures({ "x", "y", "z" });

		std::list<std::string> result = { "x", "y", "z" };

		Query q;
		Entity e = { PROCEDURE, "1" };
		q.setSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}
}