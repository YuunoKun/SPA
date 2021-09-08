#include "pch.h"

#include "QueryEvaluator.h"
#include "PKBAdapter.h"
#include "PKB.h"
#include "Common.h"

namespace UnitTesting {

	class QueryEvaluatorTest : public testing::Test {
	protected:
		QueryEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addConstant({ c1 });
			PKB::getInstance().addConstant({ c2 });
			PKB::getInstance().addConstant({ c3 });
			PKB::getInstance().addProcedure(p1);
			PKB::getInstance().addProcedure(p2);
			PKB::getInstance().addProcedure(p3);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addFollows(1, 2);
			PKB::getInstance().addFollows(2, 3);
		}

		~QueryEvaluatorTest() override {
			PKB::getInstance().resetCache();
		}

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		constant c1 = 1;
		constant c2 = 2;
		constant c3 = 3;
		std::string c1s = std::to_string(c1);
		std::string c2s = std::to_string(c2);
		std::string c3s = std::to_string(c3);
		proc_name p1 = "main";
		proc_name p2 = "sub";
		proc_name p3 = "sub1";

		std::string IF1 = "1";
		std::string IF2 = "2";
		std::string WHILE1 = "3";
		std::string	WHILE2 = "4";
		std::string READ1 = "5";
		std::string READ2 = "6";
		std::string PRINT1 = "7";
		std::string PRINT2 = "8";
		std::string ASSIGN1 = "9";
		std::string ASSIGN2 = "10";
		std::string CALL1 = "11";
		std::string CALL2 = "12";

		std::list<std::string> stmts = { IF1, IF2, WHILE1, WHILE2, READ1, READ2, 
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };
		PKBAdapter pkb;
		QueryEvaluator evaluator;
	};

	//Evaluator Select statement without any relation/Pattern
	TEST_F(QueryEvaluatorTest, evaluateQueryRaw) {
		// select v
		std::list<std::string> result1 = { x, y, z };
		Entity selected1 = { VARIABLE, Synonym{"v"} };
		// select c
		std::list<std::string> result2 = { c1s, c2s, c3s };
		Entity selected2 = { CONSTANT, Synonym{"c"} };
		// select p
		std::list<std::string> result3 = { p1, p2, p3 };
		Entity selected3 = { PROCEDURE, Synonym{"p"} };
		// select s
		std::list<std::string> result4 = stmts;
		Entity selected4 = { STMT, Synonym{"s"} };
		// select ifs
		std::list<std::string> result5 = { IF1, IF2 };
		Entity selected5 = { IF, Synonym{"f"} };
		// select w
		std::list<std::string> result6 = { WHILE1, WHILE2 };
		Entity selected6 = { WHILE, Synonym{"w"} };
		// select read
		std::list<std::string> result7 = { READ1, READ2 };
		Entity selected7 = { READ, Synonym{"r"} };
		// select print
		std::list<std::string> result8 = { PRINT1, PRINT2 };
		Entity selected8 = { PRINT, Synonym{"print"} };
		// select assign
		std::list<std::string> result9 = { ASSIGN1, ASSIGN2 };
		Entity selected9 = { ASSIGN, Synonym{"a"} };
		// select call
		std::list<std::string> result10 = { CALL1, CALL2 };
		Entity selected10 = { CALL, Synonym{"call"} };

		std::vector<std::pair<std::list<std::string>, Entity>> v = { {result1, selected1},
			{result2, selected2}, {result3, selected3}, {result4, selected4},
			{result5, selected5}, {result6, selected6}, {result7, selected7},
			{result8, selected8}, {result9, selected9}, {result10, selected10} };

		for (int i = 0; i < v.size(); i++) {
			Query q;
			q.addSelected(v[i].second);
			EXPECT_EQ(evaluator.evaluateQuery(q), v[i].first) << "Error at results : " << i + 1;
		}
	}


	//Evaluator Select statement for Follows Relation
	TEST_F(QueryEvaluatorTest, evaluateQueryFollow) {
		// select v
		std::list<std::string> result1 = { x, y, z };
		Entity selected1 = { VARIABLE, Synonym{"v"} };
		// select c
		std::list<std::string> result2 = { c1s, c2s, c3s };
		Entity selected2 = { CONSTANT, Synonym{"c"} };
		// select p
		std::list<std::string> result3 = { p1, p2, p3 };
		Entity selected3 = { PROCEDURE, Synonym{"p"} };
		// select s
		std::list<std::string> result4 = stmts;
		Entity selected4 = { STMT, Synonym{"s"} };
		// select ifs
		std::list<std::string> result5 = { IF1, IF2 };
		Entity selected5 = { IF, Synonym{"f"} };
		// select w
		std::list<std::string> result6 = { WHILE1, WHILE2 };
		Entity selected6 = { WHILE, Synonym{"w"} };
		// select read
		std::list<std::string> result7 = { READ1, READ2 };
		Entity selected7 = { READ, Synonym{"r"} };
		// select print
		std::list<std::string> result8 = { PRINT1, PRINT2 };
		Entity selected8 = { PRINT, Synonym{"print"} };
		// select assign
		std::list<std::string> result9 = { ASSIGN1, ASSIGN2 };
		Entity selected9 = { ASSIGN, Synonym{"a"} };
		// select call
		std::list<std::string> result10 = { CALL1, CALL2 };
		Entity selected10 = { CALL, Synonym{"call"} };

		std::vector<std::pair<std::list<std::string>, Entity>> v = { {result1, selected1},
			{result2, selected2}, {result3, selected3}, {result4, selected4},
			{result5, selected5}, {result6, selected6}, {result7, selected7},
			{result8, selected8}, {result9, selected9}, {result10, selected10} };

		for (int i = 0; i < v.size(); i++) {
			Query q;
			q.addSelected(v[i].second);
			EXPECT_EQ(evaluator.evaluateQuery(q), v[i].first) << "Error at results : " << i + 1;
		}
	}
}