#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"
#include "ExprParser.h"

namespace UnitTesting {
	class QueryEvaluatorTupleTest : public testing::Test {
	protected:
		QueryEvaluatorTupleTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addFollows(1, 2);
			PKB::getInstance().addFollows(2, 3);
			PKB::getInstance().addFollows(3, 4);
			PKB::getInstance().addParent(5, 6);
			PKB::getInstance().addParent(6, 7);
			PKB::getInstance().addParent(7, 8);
		}

		~QueryEvaluatorTupleTest() override {
			PKB::getInstance().resetCache();
		}

		Query initQuery(std::vector<RelRef> relation, std::vector<Entity> selected) {
			Query q;
			for (auto& r : relation) {
				q.addRelation(r);
			}

			for (auto& s : selected) {
				q.addSelected(s);
			}
			return q;
		}

		PKBAdapter pkb;
		QueryEvaluator evaluator;
	};

	TEST_F(QueryEvaluatorTupleTest, evaluateQueryTupleWithMultiTable) {
		Entity e1 = { STMT, Synonym{"e1"} };
		Entity e2 = { STMT, Synonym{"e2"} };
		Entity e3 = { STMT, Synonym{"e3"} };
		Entity e4 = { STMT, Synonym{"e4"} };
		RelRef q1(RelRef(FOLLOWS, e1, e2));
		RelRef q2(RelRef(PARENT, e3, e4));

		Query q = initQuery({ q1,q2 }, { e1, e2 });
		std::list<std::string> result= {"1 2", "2 3", "3 4"};
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e1 });
		result = { "1 1", "2 2", "3 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e2, e2 });
		result = { "2 2", "3 3", "4 4"};
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e1 });
		result = { "1 2 1", "2 3 2", "3 4 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e1, e2 });
		result = { "1 2 1 2", "2 3 2 3", "3 4 3 4" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e1, e2, e1 });
		result = { "1 2 1 2 1", "2 3 2 3 2", "3 4 3 4 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e1, e2, e1, e2 });
		result = { "1 2 1 2 1 2", "2 3 2 3 2 3", "3 4 3 4 3 4" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e3 });
		result = { "1 5", "1 6", "1 7", "2 5", "2 6", "2 7", "3 5", "3 6", "3 7" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e4 });
		result = { "1 6", "1 7", "1 8", "2 6", "2 7", "2 8", "3 6", "3 7", "3 8" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3 });
		result = { "1 2 5", "1 2 6", "1 2 7", "2 3 5", "2 3 6", "2 3 7", "3 4 5", "3 4 6", "3 4 7" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e1 });
		result = { "1 2 5 1", "1 2 6 1", "1 2 7 1", "2 3 5 2", "2 3 6 2", "2 3 7 2", "3 4 5 3", "3 4 6 3", "3 4 7 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4 });
		result = { "1 2 5 6", "1 2 6 7", "1 2 7 8", "2 3 5 6", "2 3 6 7", "2 3 7 8", "3 4 5 6", "3 4 6 7", "3 4 7 8" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4, e1 });
		result = { "1 2 5 6 1", "1 2 6 7 1", "1 2 7 8 1", "2 3 5 6 2", "2 3 6 7 2", "2 3 7 8 2", "3 4 5 6 3", "3 4 6 7 3", "3 4 7 8 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4, e1, e2});
		result = { "1 2 5 6 1 2", "1 2 6 7 1 2", "1 2 7 8 1 2", "2 3 5 6 2 3", 
			"2 3 6 7 2 3", "2 3 7 8 2 3", "3 4 5 6 3 4", "3 4 6 7 3 4", "3 4 7 8 3 4" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4, e1, e2, e3 });
		result = { "1 2 5 6 1 2 5", "1 2 6 7 1 2 6", "1 2 7 8 1 2 7", "2 3 5 6 2 3 5",
			"2 3 6 7 2 3 6", "2 3 7 8 2 3 7", "3 4 5 6 3 4 5", "3 4 6 7 3 4 6", "3 4 7 8 3 4 7" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4, e1, e2, e3, e4 });
		result = { "1 2 5 6 1 2 5 6", "1 2 6 7 1 2 6 7", "1 2 7 8 1 2 7 8", "2 3 5 6 2 3 5 6",
			"2 3 6 7 2 3 6 7", "2 3 7 8 2 3 7 8", "3 4 5 6 3 4 5 6", "3 4 6 7 3 4 6 7", "3 4 7 8 3 4 7 8" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1, q2 }, { e1, e2, e3, e4, e1, e2, e3, e4, e1});
		result = { "1 2 5 6 1 2 5 6 1", "1 2 6 7 1 2 6 7 1", "1 2 7 8 1 2 7 8 1", "2 3 5 6 2 3 5 6 2",
			"2 3 6 7 2 3 6 7 2", "2 3 7 8 2 3 7 8 2", "3 4 5 6 3 4 5 6 3", "3 4 6 7 3 4 6 7 3", "3 4 7 8 3 4 7 8 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
	}


	TEST_F(QueryEvaluatorTupleTest, evaluateQueryTupleWithRawResult) {
		Entity r1 = { READ, Synonym{"r1"} };
		Entity r2 = { READ, Synonym{"r2"} };
		Entity r3 = { READ, Synonym{"r3"} };

		Query q = initQuery({ }, { r1, r1 });
		std::list<std::string> result = { "8 8","9 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ }, { r1, r2 });
		result = { "8 8","8 9", "9 8", "9 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ }, { r1, r2 });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ }, { r1, r2, r3 });
		result = { "8 8 8","8 8 9", "8 9 8", "8 9 9", "9 8 8","9 8 9", "9 9 8", "9 9 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ }, { r1, r2, r3 });
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


	}


	TEST_F(QueryEvaluatorTupleTest, evaluateQueryTupleWithRawResultAndTable) {
		Entity e1 = { STMT, Synonym{"e1"} };
		Entity e2 = { STMT, Synonym{"e2"} };
		Entity r1 = { READ, Synonym{"r1"} };
		Entity r2 = { READ, Synonym{"r2"} };
		Entity r3 = { READ, Synonym{"r3"} };
		RelRef q1(RelRef(FOLLOWS, e1, e2));

		Query q = initQuery({ q1 }, { e1, r1 });
		std::list<std::string> result = { "1 8", "1 9", "2 8", "2 9", "3 8", "3 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ q1 }, { e1, e2, r1 });
		result = { "1 2 8", "1 2 9", "2 3 8", "2 3 9", "3 4 8", "3 4 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1 }, { e1, e2, r1, e1 });
		result = { "1 2 8 1", "1 2 9 1", "2 3 8 2", "2 3 9 2", "3 4 8 3", "3 4 9 3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1 }, { e1, e2, r1, r1 });
		result = { "1 2 8 8", "1 2 9 9", "2 3 8 8", "2 3 9 9", "3 4 8 8", "3 4 9 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ q1 }, { e1, e2, r1, r2 });
		result = { "1 2 8 8", "1 2 8 9", "1 2 9 8", "1 2 9 9", "2 3 8 8", "2 3 8 9", "2 3 9 8", "2 3 9 9", "3 4 8 8", "3 4 8 9", "3 4 9 8", "3 4 9 9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
	}
}