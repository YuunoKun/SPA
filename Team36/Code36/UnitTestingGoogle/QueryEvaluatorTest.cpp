#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {
	TEST(QueryEvaluator, evaluateQueryVariable) {
		//PKB::getInstance().setStmts({ {STMT_PRINT,1 }, {STMT_PRINT,2}, {STMT_PRINT,3} });
		PKB::getInstance().resetCache();
		PKB::getInstance().addVariable({ "a" });
		PKB::getInstance().addVariable({ "b" });
		PKB::getInstance().addVariable({ "c" });

		std::list<std::string> result = { "a", "b", "c" };

		Query q;
		Entity e = { VARIABLE, Synonym{"1"} };
		q.addSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryConstant) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addConstant({ 1 });
		PKB::getInstance().addConstant({ 2 });
		PKB::getInstance().addConstant({ 3 });

		std::list<std::string> result = { "1", "2", "3" };

		Query q;
		Entity e = { CONSTANT, Synonym{"1"} };
		q.addSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}

	TEST(QueryEvaluator, evaluateQueryProcedures) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addProcedure("x");
		PKB::getInstance().addProcedure("y");
		PKB::getInstance().addProcedure("z");

		std::list<std::string> result = { "x", "y", "z" };

		Query q;
		Entity e = { PROCEDURE, Synonym{"1"} };
		q.addSelected(e);

		QueryEvaluator qe;
		EXPECT_EQ(result, qe.evaluateQuery(q));
	}
}