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


	TEST_F(QueryEvaluatorWithTest, evaluateQueryWithClausesSynonymAndConstant) {
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

	TEST_F(QueryEvaluatorWithTest, evaluateQueryWithClausesConstantAndConstant) {
		Entity s = { STMT, Synonym{"a"} };
		Query q = initQuery({ WITH, s , { STMT, "1" } }, { s });
		std::list<std::string> result = { "1" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		q = initQuery({ WITH, { STMT, "2" }, s }, { s });
		result = { "2" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


		s = { WHILE, Synonym{"a"} };
		q = initQuery({ WITH, { WHILE, "1" } , s }, { s });
		result = { "1" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { WHILE, "2" } }, { s });
		result = { "2" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { WHILE, "3" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { IF, Synonym{"a"} };
		q = initQuery({ WITH, { IF, "3" } , s }, { s });
		result = { "3" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { IF, "4" } }, { s });
		result = { "4" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { IF, "1" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { PRINT, Synonym{"a"} };
		q = initQuery({ WITH, { PRINT, "5" } , s }, { s });
		result = { "5" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { PRINT, "6" } }, { s });
		result = { "6" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { PRINT, "7" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { PRINT, Synonym{"a"}, VAR_NAME };
		q = initQuery({ WITH, { PRINT, s1, VAR_NAME } , s }, { s });
		result = { s1 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { PRINT, s2, VAR_NAME } }, { s });
		result = { s2 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { PRINT, s3, VAR_NAME } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { READ, Synonym{"a"} };
		q = initQuery({ WITH, { READ, "7" } , s }, { s });
		result = { "7" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { READ, "8" } }, { s });
		result = { "8" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { READ, "1" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { READ, Synonym{"a"}, VAR_NAME };
		q = initQuery({ WITH, { READ, s1, VAR_NAME  } , s }, { s });
		result = { s1 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { READ, s2, VAR_NAME  } }, { s });
		result = { s2 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { READ, s3, VAR_NAME } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { CALL, Synonym{"a"} };
		q = initQuery({ WITH, { CALL, "9" } , s }, { s });
		result = { "9" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { CALL, "10" } }, { s });
		result = { "10" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { CALL, "1" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { CALL, Synonym{"a"}, PROC_NAME };
		q = initQuery({ WITH, { CALL, s1, PROC_NAME } , s }, { s });
		result = { s1 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { CALL, s2, PROC_NAME } }, { s });
		result = { s2 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { CALL, s3, PROC_NAME } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { VARIABLE, Synonym{"a"} };
		q = initQuery({ WITH, { VARIABLE, s1 } , s }, { s });
		result = { s1 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { VARIABLE, s2 } }, { s });
		result = { s2 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { VARIABLE, s3 } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { PROCEDURE, Synonym{"a"} };
		q = initQuery({ WITH, { PROCEDURE, s1 } , s }, { s });
		result = { s1 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { PROCEDURE, s2 } }, { s });
		result = { s2 };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { PROCEDURE, s3 } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);

		s = { CONSTANT, Synonym{"a"} };
		q = initQuery({ WITH, { CONSTANT, "1" } , s }, { s });
		result = { "1" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, s, { CONSTANT, "2" } }, { s });
		result = { "2" };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);
		q = initQuery({ WITH, { CONSTANT, "3" } , s }, { s });
		result = { };
		EXPECT_EQ(evaluator.evaluateQuery(q), result);


	}


	TEST_F(QueryEvaluatorWithTest, evaluateQueryWithClausesSynonymAndSynonym) {

		std::vector<Entity> headers = { { WHILE, Synonym{"a"} }, { WHILE, Synonym{"a"} } };
		Query q = initQuery({ WITH, headers[0], headers[1] }, headers);
		std::list<std::string> result = { "1 1", "2 2"};
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ WITH, headers[0], headers[1] }, headers)), result);


		headers = { { WHILE, Synonym{ "a" } }, { IF, Synonym{"b"} } };
		result = {  };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ WITH, headers[0], headers[1] }, headers)), result);

		headers = { { PRINT, Synonym{ "a" } }, { PRINT, Synonym{ "a" }, VAR_NAME },
			{ READ, Synonym{"b"}  }, { READ, Synonym{"b"} , VAR_NAME } };
		result = { "5 x 7 x","6 y 8 y" };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ WITH, headers[1], headers[3] }, headers)), result);

		headers = { { PROCEDURE, Synonym{ "a" }, PROC_NAME }, { CALL, Synonym{"b"} },  { CALL, Synonym{"b"} , PROC_NAME } };
		result = { "x 9 x","y 10 y" };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ WITH, headers[0], headers[2] }, headers)), result);

		headers = { { WHILE, Synonym{ "a" } }, { CONSTANT, Synonym{"b"} } };
		result = { "1 1","2 2" };
		EXPECT_EQ(evaluator.evaluateQuery(initQuery({ WITH, headers[0], headers[1] }, headers)), result);
	}
}