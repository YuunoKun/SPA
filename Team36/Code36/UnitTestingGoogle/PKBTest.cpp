#include "pch.h"

#include "PKB.h"
#include "PKB.cpp"
#include "Common.h"
#include <iostream>

namespace UnitTesting {
	class PKBTest : public ::testing::Test {
	protected:
		PKBTest() {
		}

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

		// Class members declared here can be used by all tests in the test suite
		// for Foo.
	};
	TEST(PKB, getConstants) {
		PKB::getInstance().resetCache();

		constant three = 3;
		constant four = 4;
		std::vector<constant> s{ three, four };
		PKB::getInstance().addConstant(three);
		PKB::getInstance().addConstant(four);
		EXPECT_EQ(s, PKB::getInstance().getConstants());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().getConstants());
	}

	TEST(PKB, getProcedures) {
		PKB::getInstance().resetCache();

		proc_name first_procedure = "main";
		proc_name second_procedure = "printY";
		std::vector<proc_name> v{ first_procedure, second_procedure };
		PKB::getInstance().addProcedure(first_procedure);
		PKB::getInstance().addProcedure(second_procedure);
		EXPECT_EQ(v, PKB::getInstance().getProcedures());
		PKB::getInstance().resetCache();
		EXPECT_NE(v, PKB::getInstance().getProcedures());
	}

	TEST(PKB, getVariables) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		std::vector<var_name> s{ x, y };
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addVariable(y);
		EXPECT_EQ(s, PKB::getInstance().getVariables());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().getVariables());
	}

	TEST(PKB, getStmts) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		std::vector<StmtInfo> s{ p1, p2, p3 };
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_EQ(s, PKB::getInstance().getStmts());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().getStmts());
	}

	TEST(PKB, getExpr) {
		PKB::getInstance().resetCache();

		auto one = std::make_pair(1, "x");
		auto two = std::make_pair(2, "x+y");
		auto three = std::make_pair(3, "x");
		auto four = std::make_pair(4, "");
		auto zero = std::make_pair(0, "");

		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);

		UniqueRelationTable<stmt_index, expr> expr_table;
		expr_table.insert(one.first, one.second);
		expr_table.insert(two.first, two.second);

		PKB::getInstance().addExprTree(one.first, one.second);
		PKB::getInstance().addExprTree(two.first, two.second);
		EXPECT_THROW(PKB::getInstance().addExprTree(three.first, three.second), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addExprTree(four.first, four.second), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addExprTree(zero.first, zero.second), std::invalid_argument);

		EXPECT_EQ(expr_table, PKB::getInstance().getExpr());
		PKB::getInstance().resetCache();
		EXPECT_NE(expr_table, PKB::getInstance().getExpr());
	}
	TEST(PKB, getParent) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_WHILE };
		StmtInfo p2{ 2, STMT_IF };
		StmtInfo p3{ 3, STMT_READ };
		RelationTable<StmtInfo, StmtInfo> forward_table;
		forward_table.insert(p1, p2);
		forward_table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		EXPECT_THROW(PKB::getInstance().addParent(3, 2), std::invalid_argument);
		EXPECT_EQ(forward_table, PKB::getInstance().getParent());
		PKB::getInstance().resetCache();
		EXPECT_NE(forward_table, PKB::getInstance().getParent());
	}

	TEST(PKB, getFollows) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		UniqueRelationTable<StmtInfo, StmtInfo> forward_table;
		forward_table.insert(p1, p2);
		forward_table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(1, 3);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_EQ(forward_table, PKB::getInstance().getFollows());
		PKB::getInstance().resetCache();
		EXPECT_NE(forward_table, PKB::getInstance().getFollows());
	}

	TEST(PKB, getUsesS) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> forward_table;
		forward_table.insert(p2, x);
		forward_table.insert(p4, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(2, x);
		EXPECT_THROW(PKB::getInstance().addUsesS(4, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(4, y);
		EXPECT_EQ(forward_table, PKB::getInstance().getUsesS());
		PKB::getInstance().resetCache();
		EXPECT_NE(forward_table, PKB::getInstance().getUsesS());
	}

	TEST(PKB, getModifiesS) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> forward_table;
		forward_table.insert(p1, x);
		forward_table.insert(p3, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		EXPECT_THROW(PKB::getInstance().addModifiesS(3, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(3, y);
		EXPECT_EQ(forward_table, PKB::getInstance().getModifiesS());
		PKB::getInstance().resetCache();
		EXPECT_NE(forward_table, PKB::getInstance().getModifiesS());
	}

	TEST(PKB, getAssignment) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		UniqueRelationTable<stmt_index, var_name> forward_table;
		forward_table.insert(1, x);

		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(2, y);
		EXPECT_EQ(forward_table, PKB::getInstance().getAssigns());
		PKB::getInstance().resetCache();
		EXPECT_NE(forward_table, PKB::getInstance().getAssigns());
	}

	TEST(PKB, resetCache) {
		PKB::getInstance().resetCache();

		PKB::getInstance().addConstant(3);
		PKB::getInstance().addProcedure("main");
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addVariable("x");
		PKB::getInstance().resetCache();
		EXPECT_EQ(PKB::getInstance().getConstants().size(), 0);
		EXPECT_EQ(PKB::getInstance().getProcedures().size(), 0);
		EXPECT_EQ(PKB::getInstance().getVariables().size(), 0);
		EXPECT_EQ(PKB::getInstance().getStmts().size(), 0);
	}
}