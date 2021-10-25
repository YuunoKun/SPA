#include "pch.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "../../source/DesignExtractor.h"
#include "../../source/PKB.h"
#include "../../source/KnowledgeBase.h"
#include "../../source/RelationTable.h"
#include "../../source/RelationTable.cpp"

/*
// Stub PKB
class StubPKB {
public:

	void addConstant(constant constant) {
		stub_const_table.insert(constant);
	}

	void addProcedure(proc_name proc_name) {
		stub_proc_table.insert(proc_name);
	}

	void addVariable(var_name var_name) {
		stub_var_table.insert(var_name);
	}

	void addStmt(StmtType stmt_type) {
		stub_stmt_table.push_back({ stub_curr_stmt_id++, stmt_type });
	}

	void addExprTree(stmt_index stmt_index, expr expr) {
		stub_expr_table.push_back({ stmt_index, expr });
	}

	void addParent(stmt_index parent, stmt_index child) {
		stub_parent_table[parent].insert(child);
	}

	void addFollows(stmt_index first, stmt_index second) {
		stub_follows_table[first] = second;
	}

	void addUsesS(stmt_index user, var_name used) {
		stub_uses_table[user].insert(used);
	}

	void addModifiesS(stmt_index modifier, var_name modified) {
		stub_modifies_table[modifier].insert(modified);
	}

	void generateParentT() {}

	void generateFollowsT() {}


	std::unordered_set<proc_name> stub_proc_table;
	std::unordered_set<var_name> stub_var_table;
	std::vector<std::pair<stmt_index, StmtType>> stub_stmt_table;
	std::vector<std::pair<stmt_index, expr>> stub_expr_table;
	std::unordered_set<constant> stub_const_table;

	std::unordered_map<stmt_index, std::unordered_set<stmt_index>> stub_parent_table;
	std::unordered_map<stmt_index, stmt_index> stub_follows_table;
	std::unordered_map<stmt_index, std::unordered_set<var_name>> stub_uses_table;
	std::unordered_map<stmt_index, std::unordered_set<var_name>> stub_modifies_table;

	unsigned int stub_curr_stmt_id{ 1 };

};
*/


namespace UnitTesting {

	using namespace SourceProcessor;


	TEST(DesignExtractor, full_behavior_test) {
		PKB::getInstance().resetCache();
		DesignExtractor* extractor = new DesignExtractor();


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
		std::vector<constant> expected_constants = {1,13,2,4,15,11};
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
		std::unordered_set<var_name> expected_stmt4_used = {"beforeIf","mainX"};
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
		std::unordered_set<var_name> expected_stmt10_used = {"whileCond","inWhile"};
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

		//ASSERT_EQ(p1->getCaller().size(), 0);

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
		//ASSERT_EQ(p1->getCaller().size(), 1);
		//ASSERT_EQ(p1->getCaller()[0], 1);


		/*
		
		// test population
		PKB stub_pkb;

		// test populate entites
		extractor->populateEntities(stub_pkb);


		std::vector<constant> expected_populated_constants = {1,13,2,4,15,11};
		ASSERT_EQ(stub_pkb.stub_const_table.size(), expected_populated_constants.size());
		for (constant c : expected_populated_constants) {
			ASSERT_TRUE(stub_pkb.stub_const_table.find(c) != stub_pkb.stub_const_table.end());
		}

		std::vector<var_name> expected_populated_variables = {
			"mainX","readVar","printVar","beforeIf",
			"mainIfCond","beforeCall","afterCall","beforeWhile",
			"whileCond","inWhile","afterWhile","afterIf","p2Var"
		};
		ASSERT_EQ(stub_pkb.stub_var_table.size(), expected_populated_variables.size());
		for (var_name v : expected_populated_variables) {
			ASSERT_TRUE(stub_pkb.stub_var_table.find(v) != stub_pkb.stub_var_table.end());
		}

		std::vector<proc_name> expected_populated_proc = { "main","p2" };
		ASSERT_EQ(stub_pkb.stub_proc_table.size(), expected_populated_proc.size());
		for (proc_name p : expected_populated_proc) {
			ASSERT_TRUE(stub_pkb.stub_proc_table.find(p) != stub_pkb.stub_proc_table.end());
		}

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
		ASSERT_EQ(stub_pkb.stub_stmt_table.size(), expected_populated_stmt.size());
		for (int i = 0; i < expected_populated_stmt.size(); i++) {
			ASSERT_EQ(stub_pkb.stub_stmt_table[i].first, expected_populated_stmt[i].first);
			ASSERT_EQ(stub_pkb.stub_stmt_table[i].second, expected_populated_stmt[i].second);
		}


		// test populate relations
		extractor->populateRelations(stub_pkb);

		// uses
		std::vector<std::vector<var_name>> expected_used_variables =
		{
			{},
			{},
			{"printVar"},
			{"beforeIf","mainX"},
			{"mainIfCond","beforeCall","p2Var","afterCall","beforeWhile","whileCond","inWhile","afterWhile"},
			{"beforeCall"},
			{"p2Var"},
			{"afterCall"},
			{"beforeWhile"},
			{"whileCond","inWhile"},
			{"inWhile"},
			{"afterWhile"},
			{"afterIf"},
			{"p2Var"}
		};
		ASSERT_EQ(stub_pkb.stub_uses_table.size(), expected_used_variables.size());
		for (int i = 0; i < expected_used_variables.size(); i++) {
			ASSERT_EQ(stub_pkb.stub_uses_table[i + 1].size(), expected_used_variables[i + 1].size());
			for (var_name v: expected_used_variables[i]) {
				ASSERT_TRUE(stub_pkb.stub_uses_table[i + 1].find(v) != stub_pkb.stub_uses_table[i + 1].end());
			}
		}

		// modifies
		std::vector<std::vector<var_name>> expected_modified_variables =
		{
			{"mainX"},
			{"readVar"},
			{},
			{"beforeIf"},
			{"beforeCall","p2Var","afterCall","beforeWhile","inWhile","afterWhile"},
			{"beforeCall"},
			{"p2Var"},
			{"afterCall"},
			{"beforeWhile"},
			{"inWhile"},
			{"inWhile"},
			{"afterWhile"},
			{"afterIf"},
			{"p2Var"}
		};
		ASSERT_EQ(stub_pkb.stub_modifies_table.size(), expected_modified_variables.size());
		for (int i = 0; i < expected_modified_variables.size(); i++) {
			ASSERT_EQ(stub_pkb.stub_modifies_table[i + 1].size(), expected_modified_variables[i + 1].size());
			for (var_name v : expected_modified_variables[i]) {
				ASSERT_TRUE(stub_pkb.stub_modifies_table[i + 1].find(v) != stub_pkb.stub_modifies_table[i + 1].end());
			}
		}
		*/


		// test population 

		// test populate entites
		PKB* pkb = &PKB::getInstance();
		extractor->populateEntities(*pkb);

		std::unordered_set<constant> expected_populated_constants = {1,13,2,4,15,11};
		ASSERT_EQ(pkb->getConstants().size(), expected_populated_constants.size());
		for (constant c : pkb->getConstants()) {
			ASSERT_TRUE(expected_populated_constants.find(c) != expected_populated_constants.end());
		}

		std::unordered_set<var_name> expected_populated_variables = {
			"mainX","readVar","printVar","beforeIf",
			"mainIfCond","beforeCall","afterCall","beforeWhile",
			"whileCond","inWhile","afterWhile","afterIf","p2Var"
		};
		ASSERT_EQ(pkb->getVariables().size(), expected_populated_variables.size());
		for (var_name v : pkb->getVariables()) {
			ASSERT_TRUE(expected_populated_variables.find(v) != expected_populated_variables.end());
		}

		std::unordered_set<proc_name> expected_populated_proc = { "main","p2" };
		ASSERT_EQ(pkb->getProcedures().size(), expected_populated_proc.size());
		for (proc_name p : pkb->getProcedures()) {
			ASSERT_TRUE(expected_populated_proc.find(p) != expected_populated_proc.end());
		}

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
		ASSERT_EQ(pkb->getStmts().size(), expected_populated_stmt.size());
		std::vector<StmtInfo> res = pkb->getStmts();
		sort(res.begin(), res.end(), [](StmtInfo s1, StmtInfo s2) { return s1.stmt_index < s2.stmt_index; });
		for (size_t i = 0; i < expected_populated_stmt.size(); i++) {
			ASSERT_EQ(res[i].stmt_index, expected_populated_stmt[i].first);
			ASSERT_EQ(res[i].stmt_type, expected_populated_stmt[i].second);
		}



		std::vector<StmtInfo> expected_stmt_info =
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

		// test populate relations
		extractor->populateRelations(*pkb);

		// uses
		std::vector<std::vector<var_name>> expected_used_variables =
		{
			{},
			{},
			{"printVar"},
			{"beforeIf","mainX"},
			{"mainIfCond","beforeCall","p2Var","afterCall","beforeWhile","whileCond","inWhile","afterWhile"},
			{"beforeCall"},
			{"p2Var"},
			{"afterCall"},
			{"beforeWhile"},
			{"whileCond","inWhile"},
			{"inWhile"},
			{"afterWhile"},
			{"afterIf"},
			{"p2Var"}
		};
		auto table_u = pkb->getUsesS();
		ASSERT_EQ(table_u.getPairs().size(), 21);
		for (size_t i = 0; i < expected_used_variables.size(); i++) {
			for (var_name v: expected_used_variables[i]) {
				ASSERT_TRUE(table_u.containsPair(expected_stmt_info[i], v));
			}
		}


		// usesP
		auto table_uP = pkb->getUsesP();
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
		ASSERT_EQ(table_uP.getPairs().size(), expected_usesP.size());
		for (auto p : expected_usesP) {
			ASSERT_TRUE(table_uP.containsPair(p.first, p.second));
		}



		// modifies
		std::vector<std::vector<var_name>> expected_modified_variables =
		{
			{"mainX"},
			{"readVar"},
			{},
			{"beforeIf"},
			{"beforeCall","p2Var","afterCall","beforeWhile","inWhile","afterWhile"},
			{"beforeCall"},
			{"p2Var"},
			{"afterCall"},
			{"beforeWhile"},
			{"inWhile"},
			{"inWhile"},
			{"afterWhile"},
			{"afterIf"},
			{"p2Var"}
		};
		auto table_m = pkb->getModifiesS();
		ASSERT_EQ(table_m.getPairs().size(), 18);
		for (size_t i = 0; i < expected_modified_variables.size(); i++) {
			for (var_name v : expected_modified_variables[i]) {
				ASSERT_TRUE(table_m.containsPair(expected_stmt_info[i], v));
			}
		}


		// modifiesP
		auto table_mP = pkb->getModifiesP();
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
		ASSERT_EQ(table_mP.getPairs().size(), expected_modifiesP.size());
		for (auto p : expected_modifiesP) {
			ASSERT_TRUE(table_mP.containsPair(p.first, p.second));
		}


		// Follows
		auto table_f = pkb->getFollows();
		std::vector<std::pair<stmt_index,stmt_index>> expected_follows =
		{
			{1,2},{2,3},{3,4},{4,5},{6,7},{7,8},
			{9,10},{10,12},{5,13}
		};
		ASSERT_EQ(table_f.getPairs().size(), expected_follows.size());
		for (auto p: expected_follows) {
			ASSERT_TRUE(table_f.containsPair(expected_stmt_info[p.first - 1], expected_stmt_info[p.second - 1]));
		}


		// Parent
		auto table_p = pkb->getParent();
		std::vector<std::pair<stmt_index, stmt_index>> expected_parent =
		{
			{5,6},{5,7},{5,8},{5,9},{5,10},{5,12},
			{10,11}
		};
		ASSERT_EQ(table_p.getPairs().size(), expected_parent.size());
		for (auto p : expected_parent) {
			ASSERT_TRUE(table_p.containsPair(expected_stmt_info[p.first - 1], expected_stmt_info[p.second - 1]));
		}


		// CallsP
		auto table_cp = pkb->getCallsP();
		std::vector<std::pair<proc_name, proc_name>> expected_cp =
		{
			{"main","p2"}
		};
		ASSERT_EQ(table_cp.getPairs().size(), expected_cp.size());
		for (auto call : expected_cp) {
			ASSERT_TRUE(table_cp.containsPair(call.first, call.second));
		}

		// CallsS
		auto table_cs = pkb->getCallsS();
		std::vector<std::pair<stmt_index, proc_name>> expected_cs =
		{
			{7,"p2"}
		};
		ASSERT_EQ(table_cs.getPairs().size(), expected_cs.size());
		for (auto call : expected_cs) {
			ASSERT_TRUE(table_cs.containsPair(call.first, call.second));
		}

		// Ifs
		auto table_ifs = pkb->getIf();
		std::vector<std::pair<stmt_index, var_name>> expected_ifs =
		{
			{5,"mainIfCond"}
		};
		ASSERT_EQ(table_ifs.getPairs().size(), expected_ifs.size());
		for (auto ifs : expected_ifs) {
			ASSERT_TRUE(table_ifs.containsPair(ifs.first, ifs.second));
		}

		// Whiles
		auto table_whiles = pkb->getWhile();
		std::vector<std::pair<stmt_index, var_name>> expected_whiles =
		{
			{10,"whileCond"}
		};
		ASSERT_EQ(table_whiles.getPairs().size(), expected_whiles.size());
		for (auto whiles : expected_whiles) {
			ASSERT_TRUE(table_whiles.containsPair(whiles.first, whiles.second));
		}


		// Nexts
		auto table_nexts = pkb->getNext();
		std::vector<std::pair<StmtInfo, StmtInfo>> expected_nexts =
		{
			{{1, STMT_ASSIGN},{2, STMT_READ}}, 
			{{2, STMT_READ},{3, STMT_PRINT}}, 
			{{3, STMT_PRINT},{4, STMT_ASSIGN}}, 
			{{4, STMT_ASSIGN},{5, STMT_IF}},
			{{5, STMT_IF},{6, STMT_ASSIGN}}, 
			{{5, STMT_IF},{9, STMT_ASSIGN}},
			{{6, STMT_ASSIGN},{7, STMT_CALL}}, 
			{{7, STMT_CALL},{8, STMT_ASSIGN}}, 
			{{8, STMT_ASSIGN},{13, STMT_ASSIGN}},
			{{9, STMT_ASSIGN},{10, STMT_WHILE}}, 
			{{10, STMT_WHILE},{12, STMT_ASSIGN}}, 
			{{10, STMT_WHILE}, {11, STMT_ASSIGN}}, 
			{{11, STMT_ASSIGN},{10, STMT_WHILE}}, 
			{{12, STMT_ASSIGN}, {13, STMT_ASSIGN}}
		};
		ASSERT_EQ(table_nexts.getPairs().size(), expected_nexts.size());
		for (const auto& next : expected_nexts) {
			ASSERT_TRUE(table_nexts.containsPair(next.first, next.second));
		}
		delete extractor;
	}


	TEST(DesignExtractor, validation_duplicate_proc_name) {
		DesignExtractor* extractor = new DesignExtractor();

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
		DesignExtractor* extractor = new DesignExtractor();

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
		DesignExtractor* extractor = new DesignExtractor();

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
		DesignExtractor* extractor = new DesignExtractor();

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