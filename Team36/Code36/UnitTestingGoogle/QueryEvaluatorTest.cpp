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
			PKB::getInstance().addParent(1, 2);
			PKB::getInstance().addParent(2, 3);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addModifiesS(3, y);
			PKB::getInstance().addUsesS(1, x);
			PKB::getInstance().addUsesS(3, y);
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

		// select v
		std::list<std::string> allVariables = { x, y, z };
		Entity selectVariable = { VARIABLE, Synonym{"v"} };
		// select c
		std::list<std::string> allConstant = { c1s, c2s, c3s };
		Entity selectConstant = { CONSTANT, Synonym{"c"} };
		// select p
		std::list<std::string> allProcedure = { p1, p2, p3 };
		Entity selectProcedure = { PROCEDURE, Synonym{"p"} };
		// select s
		std::list<std::string> allStmt = stmts;
		Entity selectStmt = { STMT, Synonym{"s"} };
		// select ifs
		std::list<std::string> allIfs = { IF1, IF2 };
		Entity selectIfs = { IF, Synonym{"f"} };
		// select w
		std::list<std::string> allWhile = { WHILE1, WHILE2 };
		Entity selectWhile = { WHILE, Synonym{"w"} };
		// select read
		std::list<std::string> allRead = { READ1, READ2 };
		Entity selectRead = { READ, Synonym{"r"} };
		// select print
		std::list<std::string> allPrint = { PRINT1, PRINT2 };
		Entity selectPrint = { PRINT, Synonym{"print"} };
		// select assign
		std::list<std::string> allAssign = { ASSIGN1, ASSIGN2 };
		Entity selectAssign = { ASSIGN, Synonym{"a"} };
		// select call
		std::list<std::string> allCall = { CALL1, CALL2 };
		Entity selectCall = { CALL, Synonym{"call"} };

		std::vector<std::list<std::string>> allResult = {
			allVariables , allConstant , allProcedure, allStmt, allIfs,
			allWhile, allRead, allPrint, allAssign, allCall };

		std::vector<Entity> allSelect = {
			selectVariable , selectConstant , selectProcedure, selectStmt, selectIfs,
			selectWhile, selectRead, selectPrint, selectAssign, selectCall };

		std::list<std::string> emptyResult = {};
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

		for (int i = 0; i < allSelect.size(); i++) {
			Query q;
			q.addSelected(allSelect[i]);
			EXPECT_EQ(evaluator.evaluateQuery(q), allResult[i]) << "Error at results : " << i + 1;
		}
	}

	//Evaluator Select statement for Follows Relation
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanTrue) {
		//Test true boolean equation
		RelRef relation1(FOLLOWS, { WILD }, { WILD });
		RelRef relation2(FOLLOWS, { STMT, "1" }, { STMT, "2" });
		RelRef relation3(FOLLOWS, { STMT, "2" }, { STMT, "3" });
		RelRef relation4(FOLLOWS, { STMT, "1" }, { WILD });
		RelRef relation5(FOLLOWS, { STMT, "2" }, { WILD });
		RelRef relation6(FOLLOWS, { WILD }, { STMT, "2" });
		RelRef relation7(FOLLOWS, { WILD }, { STMT, "3" });
		std::vector<RelRef> relations1 = { relation1, relation2, relation3, relation4,
				relation5, relation6, relation7 };

		for (int i = 0; i < relations1.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations1[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), allResult[j]) << "Error at results : " << i + 1;
			}
		}
	}

	TEST_F(QueryEvaluatorTest, evaluateQueryFollowBooleanFalse) {
		//Test false boolean equation
		RelRef relation1(FOLLOWS, { STMT, "2" }, { STMT, "2" });
		RelRef relation2(FOLLOWS, { STMT, "2" }, { STMT, "1" });
		RelRef relation3(FOLLOWS, { STMT, "1" }, { STMT, "3" });
		RelRef relation4(FOLLOWS, { STMT, "1" }, { STMT, "4" });
		RelRef relation5(FOLLOWS, { STMT, "3" }, { WILD });
		RelRef relation6(FOLLOWS, { WILD }, { STMT, "1" });
		RelRef relation7(FOLLOWS, { STMT, "4" }, { WILD });
		RelRef relation8(FOLLOWS, { WILD }, { STMT, "5" });
		RelRef relation9(FOLLOWS, { STMT, "5" }, { WILD });
		RelRef relation10(FOLLOWS, { WILD }, { STMT, "6" });
		std::vector<RelRef> relations2 = { relation1, relation2, relation3,
			relation4, relation5, relation6, relation7, relation8, relation9, relation10 };

		for (int i = 0; i < relations2.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations2[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterEmpty) {
		RelRef relation1(FOLLOWS, { STMT, "3" }, { STMT, Synonym{"a"} });
		RelRef relation2(FOLLOWS, { STMT, "4" }, { STMT, Synonym{"a"} });
		RelRef relation3(FOLLOWS, { STMT, "5" }, { STMT, Synonym{"a"} });
		RelRef relation4(FOLLOWS, { STMT, "6" }, { STMT, Synonym{"a"} });
		RelRef relation5(FOLLOWS, { STMT, "7" }, { STMT, Synonym{"a"} });
		RelRef relation6(FOLLOWS, { STMT, "8" }, { STMT, Synonym{"a"} });
		RelRef relation7(FOLLOWS, { STMT, "9" }, { STMT, Synonym{"a"} });
		RelRef relation8(FOLLOWS, { STMT, "10" }, { STMT, Synonym{"a"} });
		RelRef relation9(FOLLOWS, { STMT, "11" }, { STMT, Synonym{"a"} });
		RelRef relation10(FOLLOWS, { STMT, "12" }, { STMT, Synonym{"a"} });
		RelRef relation11(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "1" });
		RelRef relation12(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "4" });
		RelRef relation13(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "5" });
		RelRef relation14(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "6" });
		RelRef relation15(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "7" });
		RelRef relation16(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "8" });
		RelRef relation17(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "9" });
		RelRef relation18(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "10" });
		RelRef relation19(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "11" });
		RelRef relation20(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "12" });

		std::vector<RelRef> relations = {
			relation1, relation2, relation3,relation4, relation5,
			relation6, relation7, relation8,relation9, relation10,
			relation11, relation12, relation13,relation14, relation15,
			relation16, relation17, relation18,relation19, relation20 };
		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[j]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}

	/*
	TEST_F(QueryEvaluatorTest, evaluateQueryFollowFilterSuccess) {
		RelRef relation1(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, Synonym{"a"} });
		RelRef relation2(FOLLOWS, { WILD }, { STMT, Synonym{"a"} });
		RelRef relation3(FOLLOWS, { STMT, Synonym{"a"} }, { WILD });
		RelRef relation4(FOLLOWS, { STMT, "1" }, { STMT, Synonym{"a"} });
		RelRef relation5(FOLLOWS, { STMT, Synonym{"a"} }, { STMT, "2" });

		std::vector<RelRef> relations = { relation1, relation2, relation3,
			relation4, relation5 };
		for (int i = 0; i < relations.size(); i++) {
			for (int j = 0; j < allSelect.size(); j++) {
				Query q;
				q.addRelation(relations[i]);
				q.addSelected(allSelect[i]);
				EXPECT_EQ(evaluator.evaluateQuery(q), emptyResult) << "Error at results : " << i + 1 << " : " << j + 1;
			}
		}
	}
	*/
}