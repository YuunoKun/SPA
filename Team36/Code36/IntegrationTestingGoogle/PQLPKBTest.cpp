#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "QuerySystem.h"

namespace UnitTesting {
	class PQLPKBTest : public testing::Test {
	protected:

		PQLPKBTest() {
			// You can do set-up work for each test here.

			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addConstant({ c1 });
			PKB::getInstance().addConstant({ c2 });
			PKB::getInstance().addConstant({ c3 });
			PKB::getInstance().addProcedure(p1);
			//PKB::getInstance().addProcedure(p2);
			//PKB::getInstance().addProcedure(p3);
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
			PKB::getInstance().addFollows(std::stoi(FOLLOW_LEFT1), std::stoi(FOLLOW_RIGHT1));
			PKB::getInstance().addFollows(std::stoi(FOLLOW_LEFT2), std::stoi(FOLLOW_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT1), std::stoi(PARENT_RIGHT1));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT1), std::stoi(PARENT_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT2), std::stoi(PARENT_RIGHT2));
			PKB::getInstance().addParent(std::stoi(PARENT_LEFT3), std::stoi(PARENT_RIGHT3));
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT1), MODIFIES_RIGHT1);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT2), MODIFIES_RIGHT2);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT3), MODIFIES_RIGHT3);
			PKB::getInstance().addModifiesS(std::stoi(MODIFIES_LEFT4), MODIFIES_RIGHT4);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT1), USES_RIGHT1);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT2), USES_RIGHT2);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT3), EXPRESSION1);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT4), EXPRESSION2);
			PKB::getInstance().generateFollowsT();
			PKB::getInstance().generateParentT();
		}

		QuerySystem qs;
		PKBAdapter pkb;

		void validateAnswer(std::list<std::string> expected, std::list<std::string> result) {

			std::list<std::string>::iterator result_it;
			std::list<std::string>::iterator expected_it;

			
			EXPECT_EQ(expected.size(), result.size());
			
			for (result_it = result.begin(), expected_it = expected.begin(); 
				result_it != result.end() && expected_it != result.end(); 
				result_it++, expected_it++) {
				
				EXPECT_EQ(*expected_it, *result_it);
			}

			//for (auto const& i: expected) {
			//	EXPECT_TRUE(i == *result_it);
			//	result_it++;
			//}
		}

		const var_name x = "x";
		const var_name y = "y";
		const var_name z = "z";
		const constant c1 = 1;
		const constant c2 = 2;
		const constant c3 = 3;
		const std::string c1s = std::to_string(c1);
		const std::string c2s = std::to_string(c2);
		const std::string c3s = std::to_string(c3);
		const proc_name p1 = "main";
		//const proc_name p2 = "sub";
		//const proc_name p3 = "sub1";

		const std::string IF1 = "1";
		const std::string IF2 = "2";
		const std::string WHILE1 = "3";
		const std::string WHILE2 = "4";
		const std::string READ1 = "5";
		const std::string READ2 = "6";
		const std::string PRINT1 = "7";
		const std::string PRINT2 = "8";
		const std::string ASSIGN1 = "9";
		const std::string ASSIGN2 = "10";
		const std::string CALL1 = "11";
		const std::string CALL2 = "12";

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };

		const std::list<std::string> IFS = { IF1, IF2 };

		const std::list<std::string> WHILES = { WHILE1, WHILE2 };

		const std::list<std::string> READS = { READ1, READ2 };

		const std::list<std::string> PRINTS = { PRINT1, PRINT2 };

		const std::list<std::string> ASSIGNS = { ASSIGN1, ASSIGN2 };

		const std::list<std::string> CALLS = { CALL1, CALL2 };

		const std::string FOLLOW_LEFT1 = "1";
		const std::string FOLLOW_LEFT2 = "2";
		const std::string FOLLOW_RIGHT1 = "2";
		const std::string FOLLOW_RIGHT2 = "3";

		const std::vector<std::string> FOLLOW_LEFTS = { FOLLOW_LEFT1, FOLLOW_LEFT2 };
		const std::vector<std::string> FOLLOW_RIGHTS = { FOLLOW_RIGHT1, FOLLOW_RIGHT2 };

		const std::string PARENT_LEFT1 = "1";
		const std::string PARENT_LEFT2 = "2";
		const std::string PARENT_LEFT3 = "3";
		const std::string PARENT_RIGHT1 = "2";
		const std::string PARENT_RIGHT2 = "3";
		const std::string PARENT_RIGHT3 = "4";

		const std::vector<std::string> PARENT_LEFTS = { PARENT_LEFT1, PARENT_LEFT2, PARENT_LEFT3 };
		const std::vector<std::string> PARENT_RIGHTS = { PARENT_RIGHT1, PARENT_RIGHT2, PARENT_RIGHT3 };

		const std::string MODIFIES_LEFT1 = "1";
		const std::string MODIFIES_LEFT2 = "3";
		const std::string MODIFIES_RIGHT1 = x;
		const std::string MODIFIES_RIGHT2 = y;

		const std::string MODIFIES_LEFT3 = ASSIGN1;
		const std::string MODIFIES_LEFT4 = ASSIGN2;
		const std::string MODIFIES_RIGHT3 = x;
		const std::string MODIFIES_RIGHT4 = y;

		const std::string EXPRESSION1 = "x";
		const std::string EXPRESSION2 = "x + (y * 5)";
		const std::string EXPRESSION_CONSTANT = "5";

		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2, MODIFIES_LEFT3, MODIFIES_LEFT4 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2, MODIFIES_RIGHT3, MODIFIES_RIGHT4 };
		const std::vector<std::string> EXPRESSIONS = { EXPRESSION1, EXPRESSION2 };

		const std::string USES_LEFT1 = "1";
		const std::string USES_LEFT2 = "3";
		const std::string USES_RIGHT1 = y;
		const std::string USES_RIGHT2 = x;

		const std::vector<std::string> USES_LEFTS = { USES_LEFT1, USES_LEFT2 };
		const std::vector<std::string> USES_RIGHTS = { USES_RIGHT1, USES_RIGHT2 };

		
		//QueryEvaluator evaluator;

		const Synonym COMMON_SYNONYM1 = { "cs1" };
		const Synonym COMMON_SYNONYM2 = { "cs2" };

		//// select v
		//const std::list<std::string> ALL_VARIABLE = { x, y, z };
		//const Entity SELECT_VARIABLE = { VARIABLE, COMMON_SYNONYM1 };
		//// select c
		//const std::list<std::string> ALL_CONSTANT = { c1s, c2s, c3s };
		//const Entity SELECT_CONSTANT = { CONSTANT, COMMON_SYNONYM1 };
		//// select p
		////const std::list<std::string> ALL_PROCEDURE = { p1, p2, p3 };
		//const std::list<std::string> ALL_PROCEDURE = { p1 };
		//const Entity SELECT_PROCEDURE = { PROCEDURE, COMMON_SYNONYM1 };
		//// select s
		//const std::list<std::string> ALL_STMT = STMTS;
		//const Entity SELECT_STMT = { STMT, COMMON_SYNONYM1 };
		//// select ifs
		//const std::list<std::string> ALL_IF = { IF1, IF2 };
		//const Entity SELECT_IF = { IF, COMMON_SYNONYM1 };
		//// select w
		//const std::list<std::string> ALL_WHILE = { WHILE1, WHILE2 };
		//const Entity SELECT_WHILE = { WHILE, COMMON_SYNONYM1 };
		//// select read
		//const std::list<std::string> ALL_READ = { READ1, READ2 };
		//const Entity SELECT_READ = { READ, COMMON_SYNONYM1 };
		//// select print
		//const std::list<std::string> ALL_PRINT = { PRINT1, PRINT2 };
		//const Entity SELECT_PRINT = { PRINT, COMMON_SYNONYM1 };
		//// select assign
		//const std::list<std::string> ALL_ASSIGN = { ASSIGN1, ASSIGN2 };
		//const Entity SELECT_ASSIGN = { ASSIGN, COMMON_SYNONYM1 };
		//// select call
		//const std::list<std::string> ALL_CALL = { CALL1, CALL2 };
		//const Entity SELECT_CALL = { CALL, COMMON_SYNONYM1 };

		//const std::vector<std::list<std::string>> ALL_RESULT = {
		//	ALL_VARIABLE , ALL_CONSTANT , ALL_PROCEDURE, ALL_STMT, ALL_IF,
		//	ALL_WHILE, ALL_READ, ALL_PRINT, ALL_ASSIGN, ALL_CALL };

		//const std::vector<Entity> ALL_SELECT = {
		//	SELECT_VARIABLE , SELECT_CONSTANT , SELECT_PROCEDURE, SELECT_STMT, SELECT_IF,
		//	SELECT_WHILE, SELECT_READ, SELECT_PRINT, SELECT_ASSIGN, SELECT_CALL };

		//const std::list<std::string> EMPTY_RESULT = {};

		//const Entity WILD_CARD = { WILD };

		//const std::vector<Entity> VALID_CONSTANT_STMT_ENTITY = { WILD_CARD,
		//	{STMT, "1"}, {STMT, "2"}, {STMT, "3"}, {STMT, "4"}, {STMT, "5"}, {STMT, "6"},
		//	{STMT, "7"}, {STMT, "8"}, {STMT, "9"}, {STMT, "10"}, {STMT, "11"}, {STMT, "12"}
		//};

		//std::vector<Entity> ALL_VARIABLES = { { VARIABLE, x }, { VARIABLE, y }, { VARIABLE, z } };

		// If the constructor and destructor are not enough for setting up
		// and cleaning up each test, you can define the following methods:

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
		}
	/*	template<typename T>
		bool compare(std::vector<T> v1, std::vector<T> v2)
		{
			std::sort(v1.begin(), v1.end());
			std::sort(v2.begin(), v2.end());
			return v1 == v2;
		}*/

		/*std::vector<StmtInfo> sample5_stmts = { { 1, STMT_ASSIGN }, { 2, STMT_CALL} , { 3, STMT_CALL },
			{ 4, STMT_READ}, { 5, STMT_PRINT }, { 6, STMT_PRINT}, { 7, STMT_ASSIGN}, { 8, STMT_CALL},
			{ 9, STMT_WHILE} , { 10, STMT_ASSIGN} , { 11, STMT_CALL} , { 12, STMT_IF},
			{ 13, STMT_ASSIGN },  { 14, STMT_ASSIGN }, { 15, STMT_ASSIGN } };*/
	};

	TEST_F(PQLPKBTest, ValidateAnswerTest) {
		validateAnswer({ "1", "2", "3"}, { "1", "2", "3" });
		validateAnswer({}, {});
	}

	TEST_F(PQLPKBTest, noSuchThatPatternStmtTest) {
		//std::list<std::string> ans_stmt = qs.processQuery("stmt s; Select s");	

		//std::list<std::string> ans_if = qs.processQuery("ifs if; Select if");

		//std::list<std::string> ans_while = qs.processQuery("while w; Select w");

		//std::list<std::string> ans_read = qs.processQuery("read r; Select r");

		//std::list<std::string> ans_assign = qs.processQuery("assign a; Select a");

		//std::list<std::string> ans_call = qs.processQuery("call c; Select c");

		//validateAnswer(STMTS, ans_stmt);
		//validateAnswer(IFS, ans_if);
		//validateAnswer(WHILES, ans_while);
		//validateAnswer(READS, ans_read);
		//validateAnswer(ASSIGNS, ans_assign);
		//validateAnswer(CALLS, ans_call);

	}

	//TEST_F(PQLPKBTest, noSuchThatPatternIfTest) {

	//	std::list<std::string> ans_if = qs.processQuery("ifs if; Select if");

	//	validateAnswer(IFS, ans_if);

	//}
	
		
	//one such that cl

	// one pattern cl

	// one st and one pattern cl
}
