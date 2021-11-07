#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "SP/DesignExtractor.h"
#include "PKB/PKBSourceInterface.h"
#include "PKB/PKB.h"

class MockPKB : public PKBSourceInterface {
public:
	void addConstant(constant constant) {
		mock_const_table.insert(constant);
	}

	void addProcedure(proc_name proc_name) {
		mock_proc_table.insert(proc_name);
	}

	void addVariable(var_name var_name) {
		mock_var_table.insert(var_name);
	}

	void addStmt(StmtType stmt_type) {
		mock_stmt_table.push_back({ mock_curr_stmt_id, stmt_type });
		mock_curr_stmt_id++;
	}

	void addExprTree(stmt_index stmt_index, expr expr) {
		mock_expr_table.push_back({ stmt_index, expr });
	}

	void addParent(stmt_index parent, stmt_index child) {
		mock_parent_table.push_back({ parent, child });
	}

	void addParentT(stmt_index parent, stmt_index child) {}

	void addFollows(stmt_index first, stmt_index second) {
		mock_follows_table.push_back({ first, second });
	}

	void addFollowsT(stmt_index first, stmt_index second) {}

	void addUsesS(stmt_index user, var_name used) {
		mock_usesS_table.push_back({ user, used });
	}

	void addModifiesS(stmt_index modifier, var_name modified) {
		mock_modifiesS_table.push_back({ modifier, modified });
	}

	void addModifiesP(proc_name proc, var_name modified) {
		mock_modifiesP_table.push_back({ proc, modified });
	}

	void addUsesP(proc_name proc, var_name used) {
		mock_usesP_table.push_back({ proc, used });
	}

	void addNext(prog_line prog_line1, prog_line prog_line2) {
		mock_next_table.push_back({ prog_line1, prog_line2 });
	}

	void addCallsP(proc_name caller, proc_name callee) {
		mock_callsP_table.push_back({ caller, callee });
	}

	void addCallsPT(proc_name, proc_name) {}

	void addCallsS(stmt_index caller_stmt, proc_name callee) {
		mock_callsS_table.push_back({ caller_stmt, callee });
	}

	void addIf(stmt_index if_stmt_index, var_name control_var) {
		mock_if_table.push_back({ if_stmt_index, control_var });
	}

	void addWhile(stmt_index while_stmt_index, var_name control_var) {
		mock_while_table.push_back({ while_stmt_index, control_var });
	}

	void addProcContains(proc_name proc, stmt_index index) {
		mock_proc_contains_table.push_back({ proc, index });
	}

	void addCFGsToDestroy(std::vector<CFG*> cfgs) {
		mock_cfgs_to_destroy = cfgs;
	}

	void addCFGBip(CFG* cfg) {
		mock_cfgs.push_back(cfg);
	}

	std::unordered_set<proc_name> mock_proc_table;
	std::vector<std::pair<stmt_index, StmtType>> mock_stmt_table;
	std::unordered_set<var_name> mock_var_table;
	std::unordered_set<constant> mock_const_table;

	std::vector<std::pair<stmt_index, expr>> mock_expr_table;

	std::vector<std::pair<stmt_index, stmt_index>> mock_parent_table;
	std::vector<std::pair<stmt_index, stmt_index>> mock_follows_table;
	std::vector<std::pair<stmt_index, var_name>> mock_usesS_table;
	std::vector<std::pair<proc_name, var_name>> mock_usesP_table;
	std::vector<std::pair<stmt_index, var_name>> mock_modifiesS_table;
	std::vector<std::pair<proc_name, var_name>> mock_modifiesP_table;
	std::vector<std::pair<stmt_index, proc_name>> mock_callsS_table;
	std::vector<std::pair<proc_name, proc_name>> mock_callsP_table;
	std::vector<std::pair<stmt_index, stmt_index>> mock_next_table;
	std::vector<CFG*> mock_cfgs;
	std::vector<CFG*> mock_cfgs_to_destroy;
	std::vector<std::pair<proc_name, stmt_index>> mock_proc_contains_table;
	std::vector<std::pair<stmt_index, var_name>> mock_if_table;
	std::vector<std::pair<stmt_index, var_name>> mock_while_table;

	stmt_index mock_curr_stmt_id{ 1 };
};

namespace UnitTesting {
	using namespace SourceProcessor;
	TEST(DesignExtractor, full_behavior_test) {
		MockPKB pkb;
		DesignExtractor* extractor = new DesignExtractor(pkb);

		/*
		* procedure main {
		1	mainX = 1;
		2	read readVar;
		3	print printVar;
		4	beforeIf = beforeIf * mainX;
		5	if(mainIfCond==13) then {
		6		beforeCall = beforeCall + 2;
		7		call p2;
		8		afterCall = afterCall + 4;
			} else {
		9		beforeWhile = beforeWhile;
		10		while(whileCond < 15) {
		11			inWhile = inWhile;
				}
		12		afterWhile = afterWhile;}
		13	afterIf = afterIf;}
		*
		* procedure p2 {
		14	p2Var = p2Var - 11;
		* }
		*/

		// procedure main {
		extractor->addProcedure("main");

		// 1  mainX = 1;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("mainX");
		extractor->addStatementModifies("mainX");
		extractor->startExpr();
		extractor->addConstant(1);
		extractor->addExprSegment("1");
		extractor->endExpr();

		// 2  read readVar;
		extractor->addStatement(TokenType::READ);
		extractor->addVariable("readVar");
		extractor->addStatementModifies("readVar");

		// 3  print printVar;
		extractor->addStatement(TokenType::PRINT);
		extractor->addVariable("printVar");
		extractor->addStatementUses("printVar");

		// 4  beforeIf = beforeIf * mainX;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("beforeIf");
		extractor->addStatementModifies("beforeIf");
		extractor->startExpr();
		extractor->addStatementUses("beforeIf");
		extractor->addStatementUses("mainX");
		extractor->addExprSegment("beforeIf*mainX");
		extractor->endExpr();

		// 5  if(mainIfCond==13) then {
		extractor->addStatement(TokenType::IF);
		extractor->setCondExpr(true);
		extractor->addVariable("mainIfCond");
		extractor->addStatementUses("mainIfCond");
		extractor->addConstant(13);
		extractor->setCondExpr(false);
		extractor->startNesting();

		// 5->6  beforeCall = beforeCall + 2;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("beforeCall");
		extractor->addStatementModifies("beforeCall");
		extractor->startExpr();
		extractor->addStatementUses("beforeCall");
		extractor->addConstant(2);
		extractor->addExprSegment("beforeCall+2");
		extractor->endExpr();

		// 5->7  call p2;
		extractor->addStatement(TokenType::CALL);
		extractor->addCallee("p2");

		// 5->8  afterCall = afterCall + 4;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("afterCall");
		extractor->addStatementModifies("afterCall");
		extractor->startExpr();
		extractor->addStatementUses("afterCall");
		extractor->addConstant(4);
		extractor->addExprSegment("afterCall+4");
		extractor->endExpr();

		// } else {
		extractor->chopNesting();

		// 5->9  beforeWhile = beforeWhile;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("beforeWhile");
		extractor->addStatementModifies("beforeWhile");
		extractor->startExpr();
		extractor->addStatementUses("beforeWhile");
		extractor->addExprSegment("beforeWhile");
		extractor->endExpr();

		// 5->10  while(whileCond < 15) {
		extractor->addStatement(TokenType::WHILE);
		extractor->setCondExpr(true);
		extractor->addVariable("whileCond");
		extractor->addStatementUses("whileCond");
		extractor->setCondExpr(false);
		extractor->addConstant(15);
		extractor->startNesting();

		// 5->10->11 inWhile = inWhile;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("inWhile");
		extractor->addStatementModifies("inWhile");
		extractor->startExpr();
		extractor->addStatementUses("inWhile");
		extractor->addExprSegment("inWhile");
		extractor->endExpr();

		// }
		extractor->endNesting();

		// 5->12 afterWhile = afterWhile;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("afterWhile");
		extractor->addStatementModifies("afterWhile");
		extractor->startExpr();
		extractor->addStatementUses("afterWhile");
		extractor->addExprSegment("afterWhile");
		extractor->endExpr();

		// }
		extractor->endNesting();

		// 13  afterIf = afterIf;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("afterIf");
		extractor->addStatementModifies("afterIf");
		extractor->startExpr();
		extractor->addStatementUses("afterIf");
		extractor->addExprSegment("afterIf");
		extractor->endExpr();

		// procedure p2 {
		extractor->addProcedure("p2");

		// 14  p2Var = p2Var - 11;
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("p2Var");
		extractor->addStatementModifies("p2Var");
		extractor->startExpr();
		extractor->addStatementUses("p2Var");
		extractor->addConstant(11);
		extractor->addExprSegment("p2Var-11");
		extractor->endExpr();

		std::vector<Procedure*> procedures = extractor->getProcedures();
		std::vector<Statement*> statements = extractor->getStatements();
		std::unordered_set<var_name> variables = extractor->getVariables();
		std::unordered_set<constant> constants = extractor->getConstants();

		ASSERT_EQ(procedures.size(), 2);
		ASSERT_EQ(statements.size(), 14);
		ASSERT_EQ(variables.size(), 13);
		ASSERT_EQ(constants.size(), 6);

		ASSERT_NO_THROW(extractor->validate());
		ASSERT_NO_THROW(extractor->populatePostValidation());

		// test constants
		std::vector<constant> expected_constants = { 1,13,2,4,15,11 };
		ASSERT_EQ(constants.size(), expected_constants.size());
		for (constant c : expected_constants) {
			ASSERT_TRUE(constants.find(c) != constants.end());
		}

		// test variables
		std::vector<var_name> expected_variables = {
			"mainX","readVar","printVar","beforeIf",
			"mainIfCond","beforeCall","afterCall","beforeWhile",
			"whileCond","inWhile","afterWhile","afterIf","p2Var"
		};
		ASSERT_EQ(variables.size(), expected_variables.size());
		for (var_name v : expected_variables) {
			ASSERT_TRUE(variables.find(v) != variables.end());
		}

		// test statements
		// stmt1 mainX = 1;
		Statement* s1 = statements[0];
		ASSERT_EQ(s1->getIndex(), 1);
		ASSERT_EQ(s1->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s1->getProcName(), "main");
		ASSERT_EQ(s1->getCallee(), "");
		ASSERT_EQ(s1->getExprStr(), "1");
		ASSERT_EQ(s1->getDirectParent(), 0);
		ASSERT_EQ(s1->getDirectChild().size(), 0);
		ASSERT_EQ(s1->getUsedVariable().size(), 0);
		ASSERT_EQ(s1->getModifiedVariable().size(), 1);

		ASSERT_EQ(s1->getModifiedVariable()[0], "mainX");

		ASSERT_EQ(s1->getStmtList(), 1);

		// stmt2 read readVar;
		Statement* s2 = statements[1];
		ASSERT_EQ(s2->getIndex(), 2);
		ASSERT_EQ(s2->getType(), StmtType::STMT_READ);
		ASSERT_EQ(s2->getProcName(), "main");
		ASSERT_EQ(s2->getCallee(), "");
		ASSERT_EQ(s2->getExprStr(), "");
		ASSERT_EQ(s2->getDirectParent(), 0);
		ASSERT_EQ(s2->getDirectChild().size(), 0);
		ASSERT_EQ(s2->getUsedVariable().size(), 0);
		ASSERT_EQ(s2->getModifiedVariable().size(), 1);

		ASSERT_EQ(s2->getModifiedVariable()[0], "readVar");

		ASSERT_EQ(s2->getStmtList(), 1);

		// stmt3 print printVar;
		Statement* s3 = statements[2];
		ASSERT_EQ(s3->getIndex(), 3);
		ASSERT_EQ(s3->getType(), StmtType::STMT_PRINT);
		ASSERT_EQ(s3->getProcName(), "main");
		ASSERT_EQ(s3->getCallee(), "");
		ASSERT_EQ(s3->getExprStr(), "");
		ASSERT_EQ(s3->getDirectParent(), 0);
		ASSERT_EQ(s3->getDirectChild().size(), 0);
		ASSERT_EQ(s3->getUsedVariable().size(), 1);
		ASSERT_EQ(s3->getModifiedVariable().size(), 0);

		ASSERT_EQ(s3->getUsedVariable()[0], "printVar");

		ASSERT_EQ(s3->getStmtList(), 1);

		// stmt4 beforeIf = beforeIf * mainX;
		Statement* s4 = statements[3];
		ASSERT_EQ(s4->getIndex(), 4);
		ASSERT_EQ(s4->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s4->getProcName(), "main");
		ASSERT_EQ(s4->getCallee(), "");
		ASSERT_EQ(s4->getExprStr(), "beforeIf*mainX");
		ASSERT_EQ(s4->getDirectParent(), 0);
		ASSERT_EQ(s4->getDirectChild().size(), 0);
		ASSERT_EQ(s4->getUsedVariable().size(), 2);
		ASSERT_EQ(s4->getModifiedVariable().size(), 1);
		std::unordered_set<var_name> expected_stmt4_used = { "beforeIf","mainX" };
		for (var_name v : s4->getUsedVariable()) {
			ASSERT_TRUE(expected_stmt4_used.find(v) != expected_stmt4_used.end());
		}
		ASSERT_EQ(s4->getModifiedVariable()[0], "beforeIf");

		ASSERT_EQ(s4->getStmtList(), 1);

		// stmt5 if(mainIfCond==13) then {....
		Statement* s5 = statements[4];
		ASSERT_EQ(s5->getIndex(), 5);
		ASSERT_EQ(s5->getType(), StmtType::STMT_IF);
		ASSERT_EQ(s5->getProcName(), "main");
		ASSERT_EQ(s5->getCallee(), "");
		ASSERT_EQ(s5->getExprStr(), "");
		ASSERT_EQ(s5->getDirectParent(), 0);
		ASSERT_EQ(s5->getDirectChild().size(), 6);
		ASSERT_EQ(s5->getUsedVariable().size(), 8);
		ASSERT_EQ(s5->getModifiedVariable().size(), 6);

		std::unordered_set<stmt_index> expected_stmt5_child = { 6,7,8,9,10,12 };
		ASSERT_EQ(s5->getDirectChild().size(), expected_stmt5_child.size());
		for (stmt_index id : s5->getDirectChild()) {
			ASSERT_TRUE(expected_stmt5_child.find(id) != expected_stmt5_child.end());
		}

		std::unordered_set<var_name> expected_stmt5_used = {
			"mainIfCond","beforeCall","p2Var","afterCall",
			"beforeWhile","whileCond","inWhile","afterWhile"
		};
		for (var_name v : s5->getUsedVariable()) {
			ASSERT_TRUE(expected_stmt5_used.find(v) != expected_stmt5_used.end());
		}

		std::unordered_set<var_name> expected_stmt5_modified = {
			"beforeCall","p2Var","afterCall","beforeWhile","inWhile","afterWhile"
		};
		for (var_name v : s5->getModifiedVariable()) {
			ASSERT_TRUE(expected_stmt5_modified.find(v) != expected_stmt5_modified.end());
		}

		ASSERT_EQ(s5->getStmtList(), 1);

		// stmt6 beforeCall = beforeCall + 2;
		Statement* s6 = statements[5];
		ASSERT_EQ(s6->getIndex(), 6);
		ASSERT_EQ(s6->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s6->getProcName(), "main");
		ASSERT_EQ(s6->getCallee(), "");
		ASSERT_EQ(s6->getExprStr(), "beforeCall+2");
		ASSERT_EQ(s6->getDirectParent(), 5);
		ASSERT_EQ(s6->getDirectChild().size(), 0);
		ASSERT_EQ(s6->getUsedVariable().size(), 1);
		ASSERT_EQ(s6->getModifiedVariable().size(), 1);
		ASSERT_EQ(s6->getUsedVariable()[0], "beforeCall");
		ASSERT_EQ(s6->getModifiedVariable()[0], "beforeCall");

		ASSERT_EQ(s6->getStmtList(), 6);

		// stmt7 call p2;
		Statement* s7 = statements[6];
		ASSERT_EQ(s7->getIndex(), 7);
		ASSERT_EQ(s7->getType(), StmtType::STMT_CALL);
		ASSERT_EQ(s7->getProcName(), "main");
		ASSERT_EQ(s7->getCallee(), "p2");
		ASSERT_EQ(s7->getExprStr(), "");
		ASSERT_EQ(s7->getDirectParent(), 5);
		ASSERT_EQ(s7->getDirectChild().size(), 0);
		ASSERT_EQ(s7->getUsedVariable().size(), 1);
		ASSERT_EQ(s7->getModifiedVariable().size(), 1);
		ASSERT_EQ(s7->getUsedVariable()[0], "p2Var");
		ASSERT_EQ(s7->getModifiedVariable()[0], "p2Var");

		ASSERT_EQ(s7->getStmtList(), 6);

		// stmt8 afterCall = afterCall + 4;
		Statement* s8 = statements[7];
		ASSERT_EQ(s8->getIndex(), 8);
		ASSERT_EQ(s8->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s8->getProcName(), "main");
		ASSERT_EQ(s8->getCallee(), "");
		ASSERT_EQ(s8->getExprStr(), "afterCall+4");
		ASSERT_EQ(s8->getDirectParent(), 5);
		ASSERT_EQ(s8->getDirectChild().size(), 0);
		ASSERT_EQ(s8->getUsedVariable().size(), 1);
		ASSERT_EQ(s8->getModifiedVariable().size(), 1);
		ASSERT_EQ(s8->getUsedVariable()[0], "afterCall");
		ASSERT_EQ(s8->getModifiedVariable()[0], "afterCall");

		ASSERT_EQ(s8->getStmtList(), 6);

		// stmt9 beforeWhile = beforeWhile;
		Statement* s9 = statements[8];
		ASSERT_EQ(s9->getIndex(), 9);
		ASSERT_EQ(s9->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s9->getProcName(), "main");
		ASSERT_EQ(s9->getCallee(), "");
		ASSERT_EQ(s9->getExprStr(), "beforeWhile");
		ASSERT_EQ(s9->getDirectParent(), 5);
		ASSERT_EQ(s9->getDirectChild().size(), 0);
		ASSERT_EQ(s9->getUsedVariable().size(), 1);
		ASSERT_EQ(s9->getModifiedVariable().size(), 1);
		ASSERT_EQ(s9->getUsedVariable()[0], "beforeWhile");
		ASSERT_EQ(s9->getModifiedVariable()[0], "beforeWhile");

		ASSERT_EQ(s9->getStmtList(), 9);

		// stmt10 while(whileCond < 15) {......
		Statement* s10 = statements[9];
		ASSERT_EQ(s10->getIndex(), 10);
		ASSERT_EQ(s10->getType(), StmtType::STMT_WHILE);
		ASSERT_EQ(s10->getProcName(), "main");
		ASSERT_EQ(s10->getCallee(), "");
		ASSERT_EQ(s10->getExprStr(), "");
		ASSERT_EQ(s10->getDirectParent(), 5);
		ASSERT_EQ(s10->getDirectChild().size(), 1);
		ASSERT_EQ(s10->getUsedVariable().size(), 2);
		ASSERT_EQ(s10->getModifiedVariable().size(), 1);
		ASSERT_EQ(s10->getDirectChild()[0], 11);
		std::unordered_set<var_name> expected_stmt10_used = { "whileCond","inWhile" };
		for (var_name v : s10->getUsedVariable()) {
			ASSERT_TRUE(expected_stmt10_used.find(v) != expected_stmt10_used.end());
		}
		ASSERT_EQ(s10->getModifiedVariable()[0], "inWhile");

		ASSERT_EQ(s10->getStmtList(), 9);

		// stmt11 inWhile = inWhile;
		Statement* s11 = statements[10];
		ASSERT_EQ(s11->getIndex(), 11);
		ASSERT_EQ(s11->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s11->getProcName(), "main");
		ASSERT_EQ(s11->getCallee(), "");
		ASSERT_EQ(s11->getExprStr(), "inWhile");
		ASSERT_EQ(s11->getDirectParent(), 10);
		ASSERT_EQ(s11->getDirectChild().size(), 0);
		ASSERT_EQ(s11->getUsedVariable().size(), 1);
		ASSERT_EQ(s11->getModifiedVariable().size(), 1);
		ASSERT_EQ(s11->getUsedVariable()[0], "inWhile");
		ASSERT_EQ(s11->getModifiedVariable()[0], "inWhile");

		ASSERT_EQ(s11->getStmtList(), 11);

		// stmt12 afterWhile = afterWhile;}
		Statement* s12 = statements[11];
		ASSERT_EQ(s12->getIndex(), 12);
		ASSERT_EQ(s12->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s12->getProcName(), "main");
		ASSERT_EQ(s12->getCallee(), "");
		ASSERT_EQ(s12->getExprStr(), "afterWhile");
		ASSERT_EQ(s12->getDirectParent(), 5);
		ASSERT_EQ(s12->getDirectChild().size(), 0);
		ASSERT_EQ(s12->getUsedVariable().size(), 1);
		ASSERT_EQ(s12->getModifiedVariable().size(), 1);
		ASSERT_EQ(s12->getUsedVariable()[0], "afterWhile");
		ASSERT_EQ(s12->getModifiedVariable()[0], "afterWhile");

		ASSERT_EQ(s12->getStmtList(), 9);

		// stmt13 beforeWhile = beforeWhile;
		Statement* s13 = statements[12];
		ASSERT_EQ(s13->getIndex(), 13);
		ASSERT_EQ(s13->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s13->getProcName(), "main");
		ASSERT_EQ(s13->getCallee(), "");
		ASSERT_EQ(s13->getExprStr(), "afterIf");
		ASSERT_EQ(s13->getDirectParent(), 0);
		ASSERT_EQ(s13->getDirectChild().size(), 0);
		ASSERT_EQ(s13->getUsedVariable().size(), 1);
		ASSERT_EQ(s13->getModifiedVariable().size(), 1);
		ASSERT_EQ(s13->getUsedVariable()[0], "afterIf");
		ASSERT_EQ(s13->getModifiedVariable()[0], "afterIf");

		ASSERT_EQ(s13->getStmtList(), 1);

		// stmt14 p2Var = p2Var - 11;
		Statement* s14 = statements[13];
		ASSERT_EQ(s14->getIndex(), 14);
		ASSERT_EQ(s14->getType(), StmtType::STMT_ASSIGN);
		ASSERT_EQ(s14->getProcName(), "p2");
		ASSERT_EQ(s14->getCallee(), "");
		ASSERT_EQ(s14->getExprStr(), "p2Var-11");
		ASSERT_EQ(s14->getDirectParent(), 0);
		ASSERT_EQ(s14->getDirectChild().size(), 0);
		ASSERT_EQ(s14->getUsedVariable().size(), 1);
		ASSERT_EQ(s14->getModifiedVariable().size(), 1);
		ASSERT_EQ(s14->getUsedVariable()[0], "p2Var");
		ASSERT_EQ(s14->getModifiedVariable()[0], "p2Var");

		ASSERT_EQ(s14->getStmtList(), 14);

		// test procedures
		// procedure1 main
		Procedure* p1 = procedures[0];
		ASSERT_EQ(p1->getName(), "main");
		ASSERT_EQ(p1->getIndex(), 1);
		std::unordered_set<var_name> expected_proc1_used = {
			"mainX","printVar","beforeIf","mainIfCond","beforeCall","p2Var",
			"afterCall","beforeWhile","whileCond","inWhile","afterWhile","afterIf"
		};
		ASSERT_EQ(p1->getUsedVariable().size(), expected_proc1_used.size());
		for (var_name v : p1->getUsedVariable()) {
			ASSERT_TRUE(expected_proc1_used.find(v) != expected_proc1_used.end());
		}

		std::unordered_set<var_name> expected_proc1_modified = {
			"mainX","readVar","beforeIf","beforeCall","p2Var",
			"afterCall","beforeWhile","inWhile","afterWhile","afterIf"
		};
		ASSERT_EQ(p1->getModifiedVariable().size(), expected_proc1_modified.size());
		for (var_name v : p1->getModifiedVariable()) {
			ASSERT_TRUE(expected_proc1_modified.find(v) != expected_proc1_modified.end());
		}

		std::unordered_set<stmt_index> expected_proc1_child = { 1,2,3,4,5,6,7,8,9,10,11,12,13 };
		ASSERT_EQ(p1->getChild().size(), expected_proc1_child.size());
		for (stmt_index id : p1->getChild()) {
			ASSERT_TRUE(expected_proc1_child.find(id) != expected_proc1_child.end());
		}

		// procedure2 p2
		Procedure* p2 = procedures[1];
		ASSERT_EQ(p2->getName(), "p2");
		ASSERT_EQ(p2->getIndex(), 2);
		ASSERT_EQ(p2->getUsedVariable().size(), 1);
		ASSERT_EQ(p2->getUsedVariable()[0], "p2Var");
		ASSERT_EQ(p2->getModifiedVariable().size(), 1);
		ASSERT_EQ(p2->getModifiedVariable()[0], "p2Var");
		ASSERT_EQ(p2->getChild().size(), 1);
		ASSERT_EQ(p2->getChild()[0], 14);

		// Test Population of entities and relations

		// test populate entites
		extractor->populateEntities();

		// test constants
		std::vector<constant> expected_populated_constants = { 1,13,2,4,15,11 };
		auto actual_constants = std::vector<constant>(pkb.mock_const_table.begin(), pkb.mock_const_table.end());
		sort(actual_constants.begin(), actual_constants.end());
		sort(expected_populated_constants.begin(), expected_populated_constants.end());
		ASSERT_EQ(actual_constants.size(), expected_populated_constants.size());
		ASSERT_EQ(actual_constants, expected_populated_constants);

		// test variables
		std::vector<var_name> expected_populated_variables = {
			"mainX","readVar","printVar","beforeIf",
			"mainIfCond","beforeCall","afterCall","beforeWhile",
			"whileCond","inWhile","afterWhile","afterIf","p2Var"
		};
		auto actual_variables = std::vector<var_name>(pkb.mock_var_table.begin(), pkb.mock_var_table.end());
		sort(actual_variables.begin(), actual_variables.end());
		sort(expected_populated_variables.begin(), expected_populated_variables.end());
		ASSERT_EQ(actual_variables.size(), expected_populated_variables.size());
		ASSERT_EQ(actual_variables, expected_populated_variables);

		// test procedures
		std::vector<proc_name> expected_populated_procedures = { "main","p2" };
		auto actual_procedures = std::vector<var_name>(pkb.mock_proc_table.begin(), pkb.mock_proc_table.end());
		sort(actual_procedures.begin(), actual_procedures.end());
		sort(expected_populated_procedures.begin(), expected_populated_procedures.end());
		ASSERT_EQ(actual_procedures.size(), expected_populated_procedures.size());
		ASSERT_EQ(actual_procedures, expected_populated_procedures);

		// test statements
		std::vector<std::pair<stmt_index, StmtType>> expected_populated_stmt =
		{
			{1, STMT_ASSIGN},
			{2, STMT_READ},
			{3, STMT_PRINT},
			{4, STMT_ASSIGN},
			{5, STMT_IF},
			{6, STMT_ASSIGN},
			{7, STMT_CALL},
			{8, STMT_ASSIGN},
			{9, STMT_ASSIGN},
			{10, STMT_WHILE},
			{11, STMT_ASSIGN},
			{12, STMT_ASSIGN},
			{13, STMT_ASSIGN},
			{14, STMT_ASSIGN}
		};
		auto actual_statements = std::vector<std::pair<stmt_index, StmtType>>(pkb.mock_stmt_table.begin(), pkb.mock_stmt_table.end());
		sort(actual_statements.begin(), actual_statements.end());
		sort(expected_populated_stmt.begin(), expected_populated_stmt.end());
		ASSERT_EQ(actual_statements.size(), expected_populated_stmt.size());
		ASSERT_EQ(actual_statements, expected_populated_stmt);

		// test populate relations
		extractor->populateRelations();

		// usesS
		std::vector<std::pair<stmt_index, var_name>> expected_usesS =
		{
			{3, "printVar"},
			{4, "beforeIf"},
			{4,"mainX"},
			{5, "mainIfCond"},
			{5, "beforeCall"},
			{5, "p2Var"},
			{5, "afterCall"},
			{5, "beforeWhile"},
			{5, "whileCond"},
			{5, "inWhile"},
			{5, "afterWhile"},
			{6, "beforeCall"},
			{7, "p2Var"},
			{8, "afterCall"},
			{9, "beforeWhile"},
			{10, "whileCond"},
			{10, "inWhile"},
			{11, "inWhile"},
			{12, "afterWhile"},
			{13, "afterIf"},
			{14, "p2Var"}
		};
		auto actual_usesS = pkb.mock_usesS_table;
		sort(actual_usesS.begin(), actual_usesS.end());
		sort(expected_usesS.begin(), expected_usesS.end());
		ASSERT_EQ(actual_usesS.size(), expected_usesS.size());
		ASSERT_EQ(actual_usesS, expected_usesS);

		// usesP
		std::vector<std::pair<proc_name, var_name>> expected_usesP =
		{
			{"main","mainX"},
			{"main","printVar"},
			{"main","beforeIf"},
			{"main","mainIfCond"},
			{"main","beforeCall"},
			{"main","afterCall"},
			{"main","beforeWhile"},
			{"main","whileCond"},
			{"main","inWhile"},
			{"main","afterWhile"},
			{"main","afterIf"},
			{"main","p2Var"},
			{"p2","p2Var"}
		};
		auto actual_usesP = pkb.mock_usesP_table;
		sort(actual_usesP.begin(), actual_usesP.end());
		sort(expected_usesP.begin(), expected_usesP.end());
		ASSERT_EQ(actual_usesP.size(), expected_usesP.size());
		ASSERT_EQ(actual_usesP, expected_usesP);

		// modifiesS
		std::vector<std::pair<stmt_index, var_name>> expected_modifiesS =
		{
			{1, "mainX"},
			{2, "readVar"},
			{4, "beforeIf"},
			{5, "beforeCall"},
			{5, "p2Var"},
			{5, "afterCall"},
			{5, "beforeWhile"},
			{5, "inWhile"},
			{5, "afterWhile"},
			{6, "beforeCall"},
			{7, "p2Var"},
			{8, "afterCall"},
			{9, "beforeWhile"},
			{10, "inWhile"},
			{11, "inWhile"},
			{12, "afterWhile"},
			{13, "afterIf"},
			{14, "p2Var"}
		};
		auto actual_modifiesS = pkb.mock_modifiesS_table;
		sort(actual_modifiesS.begin(), actual_modifiesS.end());
		sort(expected_modifiesS.begin(), expected_modifiesS.end());
		ASSERT_EQ(actual_modifiesS.size(), expected_modifiesS.size());
		ASSERT_EQ(actual_modifiesS, expected_modifiesS);

		// modifiesP
		std::vector<std::pair<proc_name, var_name>> expected_modifiesP =
		{
			{"main","mainX"},
			{"main","readVar"},
			{"main","beforeIf"},
			{"main","beforeCall"},
			{"main","afterCall"},
			{"main","beforeWhile"},
			{"main","inWhile"},
			{"main","afterWhile"},
			{"main","afterIf"},
			{"main","p2Var"},
			{"p2","p2Var"}
		};
		auto actual_modifiesP = pkb.mock_modifiesP_table;
		sort(actual_modifiesP.begin(), actual_modifiesP.end());
		sort(expected_modifiesP.begin(), expected_modifiesP.end());
		ASSERT_EQ(actual_modifiesP.size(), expected_modifiesP.size());
		ASSERT_EQ(actual_modifiesP, expected_modifiesP);

		// Follows
		std::vector<std::pair<stmt_index, stmt_index>> expected_follows =
		{
			{1,2},{2,3},{3,4},{4,5},{6,7},{7,8},
			{9,10},{10,12},{5,13}
		};
		auto actual_follows = pkb.mock_follows_table;
		sort(actual_follows.begin(), actual_follows.end());
		sort(expected_follows.begin(), expected_follows.end());
		ASSERT_EQ(actual_follows.size(), expected_follows.size());
		ASSERT_EQ(actual_follows, expected_follows);

		// Parent
		std::vector<std::pair<stmt_index, stmt_index>> expected_parent =
		{
			{5,6},{5,7},{5,8},{5,9},{5,10},{5,12},
			{10,11}
		};
		auto actual_parent = pkb.mock_parent_table;
		sort(actual_parent.begin(), actual_parent.end());
		sort(expected_parent.begin(), expected_parent.end());
		ASSERT_EQ(actual_parent.size(), expected_parent.size());
		ASSERT_EQ(actual_parent, expected_parent);

		// CallsP
		std::vector<std::pair<proc_name, proc_name>> expected_callsP =
		{
			{"main","p2"}
		};
		auto actual_callsP = pkb.mock_callsP_table;
		sort(actual_callsP.begin(), actual_callsP.end());
		sort(expected_callsP.begin(), expected_callsP.end());
		ASSERT_EQ(actual_callsP.size(), expected_callsP.size());
		ASSERT_EQ(actual_callsP, expected_callsP);

		// CallsS
		std::vector<std::pair<stmt_index, proc_name>> expected_callsS =
		{
			{7,"p2"}
		};
		auto actual_callsS = pkb.mock_callsS_table;
		sort(actual_callsS.begin(), actual_callsS.end());
		sort(expected_callsS.begin(), expected_callsS.end());
		ASSERT_EQ(actual_callsS.size(), expected_callsS.size());
		ASSERT_EQ(actual_callsS, expected_callsS);

		// Ifs
		std::vector<std::pair<stmt_index, var_name>> expected_ifs =
		{
			{5,"mainIfCond"}
		};
		auto actual_ifs = pkb.mock_if_table;
		sort(actual_ifs.begin(), actual_ifs.end());
		sort(expected_ifs.begin(), expected_ifs.end());
		ASSERT_EQ(actual_ifs.size(), expected_ifs.size());
		ASSERT_EQ(actual_ifs, expected_ifs);

		// Whiles
		std::vector<std::pair<stmt_index, var_name>> expected_whiles =
		{
			{10,"whileCond"}
		};
		auto actual_whiles = pkb.mock_while_table;
		sort(actual_whiles.begin(), actual_whiles.end());
		sort(expected_whiles.begin(), expected_whiles.end());
		ASSERT_EQ(actual_whiles.size(), expected_whiles.size());
		ASSERT_EQ(actual_whiles, expected_whiles);

		// Nexts
		std::vector<std::pair<stmt_index, stmt_index>> expected_nexts =
		{
			{1, 2}, {2, 3}, {3, 4}, {4, 5},
			{5, 6}, {5, 9}, {6, 7}, {7, 8},
			{8, 13}, {9, 10}, {10, 12}, {10, 11},
			{11, 10}, {12, 13}
		};
		auto actual_nexts = pkb.mock_next_table;
		sort(actual_nexts.begin(), actual_nexts.end());
		sort(expected_nexts.begin(), expected_nexts.end());
		ASSERT_EQ(actual_nexts.size(), expected_nexts.size());
		ASSERT_EQ(actual_nexts, expected_nexts);

		// CFGs
		auto table_cfg = pkb.mock_cfgs;
		ASSERT_EQ(table_cfg.size(), 1);
		LabelledProgLine expected_lpl = { 1,{0} };
		ASSERT_EQ(table_cfg[0]->getHeadLabelledProgLine(), expected_lpl);

		delete extractor;
	}

	TEST(DesignExtractor, validation_duplicate_proc_name) {
		DesignExtractor* extractor = new DesignExtractor(PKB::getInstance());

		/*
		* procedure p {
		1	x = x;
		  }
		*
		* procedure p {
		2	y = y;
		* }
		*/

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("x");
		extractor->addStatementModifies("x");
		extractor->startExpr();
		extractor->addVariable("x");
		extractor->addStatementUses("x");
		extractor->addExprSegment("x");
		extractor->endExpr();

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("y");
		extractor->addStatementModifies("y");
		extractor->startExpr();
		extractor->addVariable("y");
		extractor->addStatementUses("y");
		extractor->addExprSegment("y");
		extractor->endExpr();

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}

	TEST(DesignExtractor, validation_undefined_proc_name) {
		DesignExtractor* extractor = new DesignExtractor(PKB::getInstance());

		/*
		* procedure p {
		1	call r;
		  }
		*
		* procedure q {
		2	y = y;
		* }
		*/

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::CALL);
		extractor->addCallee("r");

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::ASSIGN);
		extractor->addVariable("y");
		extractor->addStatementModifies("y");
		extractor->startExpr();
		extractor->addVariable("y");
		extractor->addStatementUses("y");
		extractor->addExprSegment("y");
		extractor->endExpr();

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}

	TEST(DesignExtractor, validation_cyclic_call) {
		DesignExtractor* extractor = new DesignExtractor(PKB::getInstance());

		/*
		* procedure p {
		1	call q;
		  }
		*
		* procedure q {
		2	call p;
		* }
		*/

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::CALL);
		extractor->addCallee("q");

		extractor->addProcedure("q");
		extractor->addStatement(TokenType::CALL);
		extractor->addCallee("p");

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}

	TEST(DesignExtractor, validation_recursive) {
		DesignExtractor* extractor = new DesignExtractor(PKB::getInstance());

		/*
		* procedure p {
		1	call p;
		  }
		*
		*/

		extractor->addProcedure("p");
		extractor->addStatement(TokenType::CALL);
		extractor->addCallee("p");

		ASSERT_THROW(extractor->validate(), std::runtime_error);
	}
}