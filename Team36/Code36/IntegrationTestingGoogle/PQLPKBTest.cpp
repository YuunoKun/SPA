#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "QuerySystem.h"
#include "ExprParser.h"

namespace UnitTesting {
	class PQLPKBTest : public testing::Test {
	protected:

		PQLPKBTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addVariable({ w });
			PKB::getInstance().addVariable({ x });
			PKB::getInstance().addVariable({ y });
			PKB::getInstance().addVariable({ z });
			PKB::getInstance().addConstant({ c1 });
			PKB::getInstance().addConstant({ c2 });
			PKB::getInstance().addConstant({ c3 });
			PKB::getInstance().addProcedure(proc1);
			PKB::getInstance().addProcedure(proc2);
			PKB::getInstance().addProcedure(proc3);
			PKB::getInstance().addProcedure(proc4);
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
			PKB::getInstance().addModifiesP(MODIFIES_P_LEFT1, MODIFIES_RIGHT1);
			PKB::getInstance().addModifiesP(MODIFIES_P_LEFT1, MODIFIES_RIGHT2);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT1), USES_RIGHT1);
			PKB::getInstance().addUsesS(std::stoi(USES_LEFT2), USES_RIGHT2);
			PKB::getInstance().addUsesP(USES_P_LEFT1, USES_RIGHT1);
			PKB::getInstance().addUsesP(USES_P_LEFT1, USES_RIGHT2);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT3), EXPRESSIONNODE_1);
			PKB::getInstance().addExprTree(std::stoi(MODIFIES_LEFT4), EXPRESSIONNODE_2);
			PKB::getInstance().generateFollowsT();
			PKB::getInstance().generateParentT();
			PKB::getInstance().addIf(std::stoi(IF_LEFT1), IF_RIGHT1);
			PKB::getInstance().addIf(std::stoi(IF_LEFT2), IF_RIGHT2);
			PKB::getInstance().addWhile(std::stoi(WHILE_LEFT1), WHILE_RIGHT1);
			PKB::getInstance().addWhile(std::stoi(WHILE_LEFT2), WHILE_RIGHT2);
			PKB::getInstance().addCallsP(proc1, proc2);
			PKB::getInstance().addCallsP(proc2, proc3);
			PKB::getInstance().generateCallsPT();
			PKB::getInstance().addNext(1, 2);
			PKB::getInstance().addNext(2, 3);
			PKB::getInstance().addNext(9, 10);
			PKB::getInstance().addProcContains(proc1, 9);
			PKB::getInstance().addProcContains(proc1, 10);
			//PKB::getInstance().addVariable(v);
			PKB::getInstance().addVariable(w);
			PKB::getInstance().addModifiesS(9, w);
			PKB::getInstance().addUsesS(10, w);
			PKB::getInstance().addCFGBip(cfg1);
			PKB::getInstance().addCFGBip(cfg2);
			PKB::getInstance().addCFGBip(cfg3);


			cfg1->add(1);
			cfg1->add(2);
			cfg1->add(3);
			cfg2->add(4);
			cfg3->add(9);
			cfg3->add(10);

		}

		QuerySystem qs;
		PKBAdapter pkb;
		ExprParser expr_parser;
		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();

		void validateAnswer(std::list<std::string> expected, std::list<std::string> result) {
			std::list<std::string>::iterator result_it;
			std::list<std::string>::iterator expected_it;

			EXPECT_EQ(expected.size(), result.size());

			for (result_it = result.begin(), expected_it = expected.begin();
				result_it != result.end() && expected_it != expected.end();
				result_it++, expected_it++) {
				EXPECT_EQ(*expected_it, *result_it);
			}

		}
		//const var_name v = "v";
		const var_name w = "w";
		const var_name x = "x";
		const var_name y = "y";
		const var_name z = "z";
		const std::list<std::string> VARS = { w, x, y, z };

		const constant c1 = 1;
		const constant c2 = 2;
		const constant c3 = 3;
		const std::string c1s = std::to_string(c1);
		const std::string c2s = std::to_string(c2);
		const std::string c3s = std::to_string(c3);
		const std::list<std::string> CONSTANTS = { c1s, c2s, c3s };

		const proc_name proc1 = "main";
		const proc_name proc2 = "sub1";
		const proc_name proc3 = "sub2";
		const proc_name proc4 = "sub3";
		const std::list<std::string> PROCEDURES = { proc1, proc2, proc3, proc4 };

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
		const std::string READ3 = "13";

		const std::list<std::string> EMPTY_RESULT = {};

		const std::list<std::string> STMTS = { IF1, IF2, WHILE1, WHILE2, READ1, READ2,
			PRINT1, PRINT2, ASSIGN1, ASSIGN2, CALL1, CALL2 };

		const std::list<std::string> IFS = { IF1, IF2 };

		const std::list<std::string> WHILES = { WHILE1, WHILE2 };

		const std::list<std::string> READS = { READ1, READ2 };

		const std::list<std::string> PRINTS = { PRINT1, PRINT2 };

		const std::list<std::string> ASSIGNS = { ASSIGN2, ASSIGN1 };

		const std::list<std::string> CALLS = { CALL1, CALL2 };

		const std::string FOLLOW_LEFT1 = "1";
		const std::string FOLLOW_LEFT2 = "2";
		const std::string FOLLOW_RIGHT1 = "2";
		const std::string FOLLOW_RIGHT2 = "3";

		//Follows(1,s) return 2
		const std::list<std::string> EXPECTED_FOLLOWS1 = { IF2 };

		//Follows(2,s) return 3
		const std::list<std::string> EXPECTED_FOLLOWS2 = { WHILE1 };

		//Follows*(1,s) return 2, 3
		const std::list<std::string> EXPECTED_FOLLOWST = { IF2, WHILE1 };

		const std::vector<std::string> FOLLOW_LEFTS = { FOLLOW_LEFT1, FOLLOW_LEFT2 };
		const std::vector<std::string> FOLLOW_RIGHTS = { FOLLOW_RIGHT1, FOLLOW_RIGHT2 };

		const std::string PARENT_LEFT1 = "1";
		const std::string PARENT_LEFT2 = "2";
		const std::string PARENT_LEFT3 = "3";
		const std::string PARENT_RIGHT1 = "2";
		const std::string PARENT_RIGHT2 = "3";
		const std::string PARENT_RIGHT3 = "4";

		//based on insertion to PKB in test constructor

		// Parent(1, s) return stmt 2,3
		const std::list<std::string> EXPECTED_PARENT1 = { IF2, WHILE1 };

		// Parent(2, s) return stmt 3
		const std::list<std::string> EXPECTED_PARENT2 = { WHILE1 };

		// Parent(3, s) return stmt 4
		const std::list<std::string> EXPECTED_PARENT3 = { WHILE2 };

		// Parent*(1, s) return stmt 2, 3, 4
		const std::list<std::string> EXPECTED_PARENTT = { IF2, WHILE1, WHILE2 };

		//Follows(s,1) and Parent(1,s) return stmt 2
		const std::list<std::string> EXPECTED_FOLLOWS_PARENT = { IF2 };

		const std::vector<std::string> PARENT_LEFTS = { PARENT_LEFT1, PARENT_LEFT2, PARENT_LEFT3 };
		const std::vector<std::string> PARENT_RIGHTS = { PARENT_RIGHT1, PARENT_RIGHT2, PARENT_RIGHT3 };

		const std::string MODIFIES_LEFT1 = "1";
		const std::string MODIFIES_LEFT2 = "3";
		const std::string MODIFIES_RIGHT1 = x;
		const std::string MODIFIES_RIGHT2 = y;
		const std::list<std::string> EXPECTED_MODIFIES1 = { x };
		const std::list<std::string> EXPECTED_MODIFIES2 = { y };
		const std::list<std::string> EXPECTED_MODIFIES3 = { x, y };

		const std::string MODIFIES_LEFT3 = ASSIGN1;
		const std::string MODIFIES_LEFT4 = ASSIGN2;
		const std::string MODIFIES_RIGHT3 = x;
		const std::string MODIFIES_RIGHT4 = y;

		const std::string MODIFIES_P_LEFT1 = proc1;


		const std::string EXPRESSION1 = "x";
		const std::string EXPRESSION2 = "x + (y * 5)";
		const std::string EXPRESSION_CONSTANT = "5";
		expr EXPRESSIONNODE_1 = expr_parser.parse(EXPRESSION1);
		expr EXPRESSIONNODE_2 = expr_parser.parse(EXPRESSION2);
		expr EXPRESSIONNODE_CONSTANT = expr_parser.parse(EXPRESSION_CONSTANT);

		const std::list<std::string> EXPECTED_ASSIGN_PATTERN1 = { ASSIGN1 };
		const std::list<std::string> EXPECTED_ASSIGN_PATTERN2 = { ASSIGN2, ASSIGN1 };
		const std::list<std::string> EXPECTED_ASSIGN_PATTERN3 = { ASSIGN2 };
		const std::list<std::string> EXPECTED_ASSIGN_PATTERN4 = { ASSIGN2 };
		const std::list<std::string> EXPECTED_ASSIGN_PATTERN5 = { ASSIGN2 };


		const std::vector<std::string> MODIFIES_LEFTS = { MODIFIES_LEFT1, MODIFIES_LEFT2, MODIFIES_LEFT3, MODIFIES_LEFT4 };
		const std::vector<std::string> MODIFIES_RIGHTS = { MODIFIES_RIGHT1, MODIFIES_RIGHT2, MODIFIES_RIGHT3, MODIFIES_RIGHT4 };

		const std::vector<std::string> EXPRESSIONS = { EXPRESSION1, EXPRESSION2 };

		const std::string USES_LEFT1 = "1";
		const std::string USES_LEFT2 = "3";
		const std::string USES_RIGHT1 = y;
		const std::string USES_RIGHT2 = x;
		const std::string USES_P_LEFT1 = proc1;


		const std::list<std::string> EXPECTED_USES1 = { y };
		const std::list<std::string> EXPECTED_USES2 = { x };
		const std::list<std::string> EXPECTED_USES3 = { y, x };


		const std::vector<std::string> USES_LEFTS = { USES_LEFT1, USES_LEFT2 };
		const std::vector<std::string> USES_RIGHTS = { USES_RIGHT1, USES_RIGHT2 };

		// for ST and pattern clauses
		const std::list<std::string> EXPECTED_ST_PATTERN1 = { ASSIGN1 };

		const std::string IF_LEFT1 = IF1;
		const std::string IF_LEFT2 = IF2;
		const std::string IF_RIGHT1 = x;
		const std::string IF_RIGHT2 = y;

		const std::vector<std::string> IF_LEFTS = { IF_LEFT1, IF_LEFT2 };
		const std::vector<std::string> IF_RIGHTS = { IF_RIGHT1, IF_RIGHT2 };
		const std::list<std::string> EXPECTED_IFS1 = { IF_LEFT1 };
		const std::list<std::string> EXPECTED_IFS2 = { IF_LEFT2 };
		const std::list<std::string> EXPECTED_IFS3 = { IF_LEFT1, IF_LEFT2 };

		const std::string WHILE_LEFT1 = WHILE1;
		const std::string WHILE_LEFT2 = WHILE2;
		const std::string WHILE_RIGHT1 = x;
		const std::string WHILE_RIGHT2 = y;

		const std::vector<std::string> WHILE_LEFTS = { WHILE_LEFT1, WHILE_LEFT2 };
		const std::vector<std::string> WHILE_RIGHTS = { WHILE_RIGHT1, WHILE_RIGHT2 };
		const std::list<std::string> EXPECTED_WHILES1 = { WHILE_LEFT1 };
		const std::list<std::string> EXPECTED_WHILES2 = { WHILE_LEFT2 };
		const std::list<std::string> EXPECTED_WHILES3 = { WHILE_LEFT1, WHILE_LEFT2 };

		const std::list<std::string> EXPECTED_P_CALLS = { proc1, proc2 };
		const std::list<std::string> EXPECTED_P_CALLS_T = { proc2, proc3 };

		const std::list<std::string> EXPECTED_NEXT = { IF2 };
		const std::list<std::string> EXPECTED_NEXT_T = { IF2, WHILE1 };

		const std::list<std::string> EXPECTED_AFFECT = { ASSIGN2 };
		const std::list<std::string> EXPECTED_AFFECT_T = { ASSIGN2 };
		const std::list<std::string> EXPECTED_AFFECT_BIP = { ASSIGN2 };
		const std::list<std::string> EXPECTED_AFFECT_BIP_T = { ASSIGN2 };

		const std::list<std::string> EXPECTED_WITH_AND = { IF2 };

		const std::list<std::string> EXPECTED_TUPLE = { "1 2" };

		const std::list<std::string> EXPECTED_BOOLEAN = { "TRUE" };

		virtual void SetUp() override {
		}

		void TearDown() override {
		}
	};

	TEST_F(PQLPKBTest, ValidateAnswerTest) {
		validateAnswer({ "1", "2", "3" }, { "1", "2", "3" });
		validateAnswer(EMPTY_RESULT, {});
	}

	TEST_F(PQLPKBTest, noSuchThatPatternStmtTest) {
		std::list<std::string> ans_stmt = qs.processQuery("stmt s; Select s");

		validateAnswer(STMTS, ans_stmt);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternIfTest) {
		std::list<std::string> ans_if = qs.processQuery("if ifs; Select ifs");

		validateAnswer(IFS, ans_if);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternWhileTest) {
		std::list<std::string> ans_while = qs.processQuery("while w; Select w");

		validateAnswer(WHILES, ans_while);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternReadTest) {
		std::list<std::string> ans_read = qs.processQuery("read r; Select r");

		validateAnswer(READS, ans_read);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternAssignTest) {
		std::list<std::string> ans_assign = qs.processQuery("assign a; Select a");

		validateAnswer(ASSIGNS, ans_assign);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternCallTest) {
		std::list<std::string> ans_call = qs.processQuery("call c; Select c");

		validateAnswer(CALLS, ans_call);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternVarTest) {
		std::list<std::string> ans_var = qs.processQuery("variable v; Select v");

		validateAnswer(VARS, ans_var);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternConstantTest) {
		std::list<std::string> ans_constant = qs.processQuery("constant c; Select c");

		validateAnswer(CONSTANTS, ans_constant);
	}

	TEST_F(PQLPKBTest, noSuchThatPatternProcedureTest) {
		std::list<std::string> ans_procedure = qs.processQuery("procedure p; Select p");

		validateAnswer(PROCEDURES, ans_procedure);
	}

	// one such that cl --------------------------------------------------------------------------------------------------------------
	TEST_F(PQLPKBTest, SuchThatModifiesSTest1) {
		std::list<std::string> ans = qs.processQuery("stmt s; variable v; Select v such that Modifies(1, v)");

		validateAnswer(EXPECTED_MODIFIES1, ans);
	}

	TEST_F(PQLPKBTest, SuchThatModifiesSTest2) {
		std::list<std::string> ans = qs.processQuery("stmt s; variable v; Select v such that Modifies(3, v)");

		validateAnswer(EXPECTED_MODIFIES2, ans);
	}

	TEST_F(PQLPKBTest, SuchThatModifiesPTest) {
		std::list<std::string> ans = qs.processQuery("procedure p; variable v; Select v such that Modifies(p, v)");

		validateAnswer(EXPECTED_MODIFIES3, ans);
	}

	TEST_F(PQLPKBTest, SuchThatUsesSTest1) {
		std::list<std::string> ans = qs.processQuery("variable v; Select v such that Uses(1,v)");

		validateAnswer(EXPECTED_USES1, ans);
	}

	TEST_F(PQLPKBTest, SuchThatUsesSTest2) {
		std::list<std::string> ans = qs.processQuery("variable v; Select v such that Uses(3,v)");

		validateAnswer(EXPECTED_USES2, ans);
	}

	TEST_F(PQLPKBTest, SuchThatUsesPTest) {
		std::list<std::string> ans = qs.processQuery("procedure p; variable v; Select v such that Uses(p,v)");

		validateAnswer(EXPECTED_USES3, ans);
	}


	TEST_F(PQLPKBTest, SuchThatParentTest1) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Parent(1,s)");

		validateAnswer(EXPECTED_PARENT1, ans);
	}

	TEST_F(PQLPKBTest, SuchThatParentTest2) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Parent(2,s)");

		validateAnswer(EXPECTED_PARENT2, ans);
	}

	TEST_F(PQLPKBTest, SuchThatParentTest3) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Parent(3,s)");

		validateAnswer(EXPECTED_PARENT3, ans);
	}

	TEST_F(PQLPKBTest, SuchThatParentTTest) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Parent*(1,s)");

		validateAnswer(EXPECTED_PARENTT, ans);
	}

	TEST_F(PQLPKBTest, SuchThatFollowsTest1) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Follows(1,s)");

		validateAnswer(EXPECTED_FOLLOWS1, ans);
	}

	TEST_F(PQLPKBTest, SuchThatFollowsTest2) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Follows(2,s)");

		validateAnswer(EXPECTED_FOLLOWS2, ans);
	}

	TEST_F(PQLPKBTest, SuchThatFollowsTTest) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Follows*(1,s)");

		validateAnswer(EXPECTED_FOLLOWST, ans);
	}

	// test for "and" in between multiple relref clauses
	TEST_F(PQLPKBTest, multipleRelRef) {
		std::list<std::string> ans = qs.processQuery("stmt s; Select s such that Follows(1,s) and Parent(1,s)");

		validateAnswer(EXPECTED_FOLLOWS_PARENT, ans);
	}


	// one pattern cl ----------------------------------------------------------------------------------------------------------------

	TEST_F(PQLPKBTest, PatternAssignTest1) {
		std::list<std::string> ans = qs.processQuery("assign a; Select a pattern a (_,\"x\")");

		validateAnswer(EXPECTED_ASSIGN_PATTERN1, ans);
	}

	TEST_F(PQLPKBTest, PatternAssignTest2) {
		std::list<std::string> ans = qs.processQuery("assign a; Select a pattern a (_,_\"x\"_)");

		validateAnswer(EXPECTED_ASSIGN_PATTERN2, ans);
	}

	TEST_F(PQLPKBTest, PatternAssignTest3) {
		std::list<std::string> ans = qs.processQuery("assign a; Select a pattern a (_,_\"5\"_)");

		validateAnswer(EXPECTED_ASSIGN_PATTERN3, ans);
	}

	TEST_F(PQLPKBTest, PatternAssignTest4) {
		std::list<std::string> ans = qs.processQuery("assign a; Select a pattern a (_,\"x + (y * 5)\")");

		validateAnswer(EXPECTED_ASSIGN_PATTERN5, ans);
	}

	TEST_F(PQLPKBTest, PatternAssignTest5) {
		std::list<std::string> ans = qs.processQuery("assign a; Select a pattern a (_,_\"x + (y * 5)\"_)");

		validateAnswer(EXPECTED_ASSIGN_PATTERN5, ans);
	}

	TEST_F(PQLPKBTest, PatternIfTest) {
		std::list<std::string> ans = qs.processQuery("if ifs; Select ifs pattern ifs(\"x\", _, _)");

		validateAnswer(EXPECTED_IFS1, ans);
	}

	TEST_F(PQLPKBTest, PatternIfTest2) {
		std::list<std::string> ans = qs.processQuery("if ifs; Select ifs pattern ifs(\"y\",_,_)");

		validateAnswer(EXPECTED_IFS2, ans);
	}

	TEST_F(PQLPKBTest, PatternIfTest3) {
		std::list<std::string> ans = qs.processQuery("if ifs; Select ifs pattern ifs(_,_,_)");

		validateAnswer(EXPECTED_IFS3, ans);
	}

	TEST_F(PQLPKBTest, PatternWhileTest) {
		std::list<std::string> ans = qs.processQuery("while w; Select w pattern w(\"x\", _)");

		validateAnswer(EXPECTED_WHILES1, ans);
	}

	TEST_F(PQLPKBTest, PatternWhileTest2) {
		std::list<std::string> ans = qs.processQuery("while w; Select w pattern w(\"y\",_)");

		validateAnswer(EXPECTED_WHILES2, ans);
	}

	TEST_F(PQLPKBTest, PatternWhileTest3) {
		std::list<std::string> ans = qs.processQuery("while w; Select w pattern w(_,_)");

		validateAnswer(EXPECTED_WHILES3, ans);
	}


	// one st and one pattern cl -----------------------------------------------------------------------------------------------------
	TEST_F(PQLPKBTest, SuchThatPatternTest1) {
		std::list<std::string> ans = qs.processQuery("stmt s; assign a; variable v; Select a such that Modifies(s, v) pattern a(_,\"x\")");

		validateAnswer(EXPECTED_ST_PATTERN1, ans);
	}

	TEST_F(PQLPKBTest, SuchThatPatternTest2) {
		std::list<std::string> ans = qs.processQuery("stmt s; assign a; variable v; Select a pattern a(_,\"x\") such that Modifies(s, v) ");

		validateAnswer(EXPECTED_ST_PATTERN1, ans);
	}

	// calls
	TEST_F(PQLPKBTest, CallsTest) {
		std::list<std::string> ans = qs.processQuery("procedure p, q; Select p such that Calls(p, q) ");

		validateAnswer(EXPECTED_P_CALLS, ans);
	}

	TEST_F(PQLPKBTest, CallsTTest) {
		std::list<std::string> ans = qs.processQuery("procedure q; Select q such that Calls*(\"main\", q) ");

		validateAnswer(EXPECTED_P_CALLS_T, ans);
	}

	TEST_F(PQLPKBTest, NextTest) {
		std::list<std::string> ans = qs.processQuery("prog_line p, q; Select q such that Next(1, q) ");

		validateAnswer(EXPECTED_NEXT, ans);
	}

	TEST_F(PQLPKBTest, NextTTest) {
		std::list<std::string> ans = qs.processQuery("prog_line p; Select p such that Next*(1, p) ");

		validateAnswer(EXPECTED_NEXT_T, ans);
	}

	TEST_F(PQLPKBTest, AffectsTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select s1 such that Affects(9, s1) ");

		validateAnswer(EXPECTED_AFFECT, ans);
	}

	TEST_F(PQLPKBTest, AffectsTTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select s1 such that Affects*(9, s1) ");

		validateAnswer(EXPECTED_AFFECT_T, ans);
	}

	TEST_F(PQLPKBTest, NextBipTest) {
		std::list<std::string> ans = qs.processQuery("prog_line p, q; Select q such that NextBip(1, q) ");

		validateAnswer(EXPECTED_NEXT, ans);
	}

	TEST_F(PQLPKBTest, NextBipTTest) {
		std::list<std::string> ans = qs.processQuery("prog_line p; Select p such that NextBip*(1, p) ");

		validateAnswer(EXPECTED_NEXT_T, ans);
	}

	TEST_F(PQLPKBTest, AffectsBipTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select s1 such that AffectsBip(9, s1) ");

		validateAnswer(EXPECTED_AFFECT_BIP, ans);
	}

	TEST_F(PQLPKBTest, AffectsBipTTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select s1 such that AffectsBip*(9, s1) ");

		validateAnswer(EXPECTED_AFFECT_BIP_T, ans);
	}

	TEST_F(PQLPKBTest, WithAndTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select s1 such that Parent(s,s1) with s.stmt# = 1 and s1.stmt# = 2");
		validateAnswer(EXPECTED_WITH_AND, ans);
	}

	TEST_F(PQLPKBTest, TupleTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select <s, s1> such that Parent(s,s1) with s.stmt# = 1 and s1.stmt# = 2");
		validateAnswer(EXPECTED_TUPLE, ans);
	}

	TEST_F(PQLPKBTest, BooleanTest) {
		std::list<std::string> ans = qs.processQuery("stmt s, s1; Select BOOLEAN such that Parent(s,s1)");
		validateAnswer(EXPECTED_BOOLEAN, ans);
	}
}

