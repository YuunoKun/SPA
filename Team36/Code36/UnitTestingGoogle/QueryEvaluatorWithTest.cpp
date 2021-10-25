#include "pch.h"


#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {
	class QueryEvaluatorWithTest : public testing::Test {
	protected:
		QueryEvaluatorWithTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addVariable(s1);
			PKB::getInstance().addVariable(s2);
			PKB::getInstance().addProcedure(s1);
			PKB::getInstance().addProcedure(s2);
			PKB::getInstance().addUsesS(5, s1);
			PKB::getInstance().addUsesS(6, s2);
			PKB::getInstance().addModifiesS(7, s1);
			PKB::getInstance().addModifiesS(8, s2);
			PKB::getInstance().addCallsS(9, s1);
			PKB::getInstance().addCallsS(10, s2);
			PKB::getInstance().addConstant(1);
			PKB::getInstance().addConstant(2);
		}

		~QueryEvaluatorWithTest() override {
			PKB::getInstance().resetCache();
		}

		Query initQuery(RelRef relation, std::vector<Entity> selected) {
			Query q;
			q.addRelation(relation);

			for (auto& s : selected) {
				q.addSelected(s);
			}
			return q;
		}

		PKBAdapter pkb;
		QueryEvaluator evaluator;

		std::string s1 = "x";
		std::string s2 = "y";
		std::string s3 = "z";
	};


	TEST_F(QueryEvaluatorWithTest, evaluateQueryWithConstant) {
		Entity c1 = { STMT, "1" };
		Entity c2 = { STMT, "2" };
		Entity c3 = { STMT, "3" };
		Entity c4 = { STMT, "1" };


		Query q = initQuery({WITH, c1, c1}, { BOOLEAN });
		std::list<std::string> result = { BOOLEAN_TRUE };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({WITH, c1, c4}, { BOOLEAN });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		result = { BOOLEAN_FALSE };
		q = initQuery({WITH, c1, c2}, { BOOLEAN });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({WITH, c1, c3}, { BOOLEAN });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({WITH, c2, c3}, { BOOLEAN });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


	}
}