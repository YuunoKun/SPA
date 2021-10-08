#include "pch.h"

#include "PKB.h"
#include "PKB.cpp"
#include "Common.h"
#include "ExprParser.h"
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
		ExprParser expr_parser;

		auto one = std::make_pair(1, expr_parser.parse("x"));
		auto two = std::make_pair(2, expr_parser.parse("x+y"));
		auto three = std::make_pair(3, expr_parser.parse("x"));
		auto four = std::make_pair(4, expr_parser.parse(""));
		auto zero = std::make_pair(0, expr_parser.parse(""));

		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);

		std::unordered_map<stmt_index, expr> expr_table;
		expr_table.insert(one);
		expr_table.insert(two);

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
		RelationTable<StmtInfo, StmtInfo> expected_table;
		expected_table.insert(p1, p2);
		expected_table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		EXPECT_THROW(PKB::getInstance().addParent(3, 2), std::invalid_argument);
		EXPECT_EQ(expected_table, PKB::getInstance().getParent());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getParent());
	}

	TEST(PKB, getParentT) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_IF };
		StmtInfo p6{ 6, STMT_READ };

		RelationTable<StmtInfo, StmtInfo> expected_table;
		expected_table.insert(p2, p3);
		expected_table.insert(p2, p4);
		expected_table.insert(p2, p5);
		expected_table.insert(p2, p6);
		expected_table.insert(p4, p5);
		expected_table.insert(p4, p6);
		expected_table.insert(p5, p6);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(2, 4);
		PKB::getInstance().addParent(4, 5);
		PKB::getInstance().addParent(5, 6);
		PKB::getInstance().generateParentT();

		auto output = PKB::getInstance().getParentT();
		EXPECT_EQ(expected_table, output);
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getParentT());
	}
	TEST(PKB, getFollows) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		UniqueRelationTable<StmtInfo, StmtInfo> expected_table;
		expected_table.insert(p1, p2);
		expected_table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(1, 3);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_EQ(expected_table, PKB::getInstance().getFollows());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getFollows());
	}

	TEST(PKB, getFollowsT) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };
		StmtInfo p5{ 5, STMT_IF };
		StmtInfo p6{ 6, STMT_READ };

		RelationTable<StmtInfo, StmtInfo> expected_table;
		expected_table.insert(p1, p2);
		expected_table.insert(p3, p4);
		expected_table.insert(p3, p5);
		expected_table.insert(p4, p5);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(3, 4);
		PKB::getInstance().addFollows(4, 5);
		PKB::getInstance().generateFollowsT();

		auto output = PKB::getInstance().getFollowsT();
		EXPECT_EQ(expected_table, output);
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getFollowsT());
	}

	TEST(PKB, getUsesS) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> expected_table;
		expected_table.insert(p2, x);
		expected_table.insert(p4, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(2, x);
		EXPECT_THROW(PKB::getInstance().addUsesS(4, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(4, y);
		EXPECT_EQ(expected_table, PKB::getInstance().getUsesS());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getUsesS());
	}

	TEST(PKB, getModifiesS) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> expected_table;
		expected_table.insert(p1, x);
		expected_table.insert(p3, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		EXPECT_THROW(PKB::getInstance().addModifiesS(3, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(3, y);
		EXPECT_EQ(expected_table, PKB::getInstance().getModifiesS());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getModifiesS());
	}

	TEST(PKB, getUsesP) {
		PKB::getInstance().resetCache();

		proc_name first = "first";
		proc_name second = "second";
		proc_name invalid = "invalid";
		var_name x = "x";
		var_name y = "y";
		RelationTable<proc_name, var_name> expected_table;
		expected_table.insert(first, x);
		expected_table.insert(first, y);
		expected_table.insert(second, y);

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesP(first, x);

		EXPECT_THROW(PKB::getInstance().addUsesP(first, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		EXPECT_NO_THROW(PKB::getInstance().addUsesP(first, y));
		EXPECT_NO_THROW(PKB::getInstance().addUsesP(second, y));

		EXPECT_THROW(PKB::getInstance().addUsesP(invalid, x), std::invalid_argument);
		PKB::getInstance().addProcedure(invalid);

		EXPECT_EQ(expected_table, PKB::getInstance().getUsesP());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getUsesP());
	}

	TEST(PKB, getModifiesP) {
		PKB::getInstance().resetCache();

		proc_name first = "first";
		proc_name second = "second";
		proc_name invalid = "invalid";
		var_name x = "x";
		var_name y = "y";
		RelationTable<proc_name, var_name> expected_table;
		expected_table.insert(first, x);
		expected_table.insert(first, y);
		expected_table.insert(second, y);

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesP(first, x);

		EXPECT_THROW(PKB::getInstance().addModifiesP(first, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		EXPECT_NO_THROW(PKB::getInstance().addModifiesP(first, y));
		EXPECT_NO_THROW(PKB::getInstance().addModifiesP(second, y));

		EXPECT_THROW(PKB::getInstance().addModifiesP(invalid, x), std::invalid_argument);
		PKB::getInstance().addProcedure(invalid);

		EXPECT_EQ(expected_table, PKB::getInstance().getModifiesP());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getModifiesP());
	}

	TEST(PKB, getCallsP) {
		PKB::getInstance().resetCache();

		proc_name first = "first";
		proc_name second = "second";
		proc_name invalid = "invalid";
		RelationTable<proc_name, proc_name> expected_table;
		expected_table.insert(first, second);

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addCallsP(first, second);

		EXPECT_THROW(PKB::getInstance().addCallsP(first, invalid), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addCallsP(invalid, first), std::invalid_argument);

		EXPECT_EQ(expected_table, PKB::getInstance().getCallsP());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getCallsP());
	}

	TEST(PKB, getCallsPT) {
		PKB::getInstance().resetCache();

		proc_name first = "first";
		proc_name second = "second";
		proc_name third = "third";
		proc_name fourth = "fourth";
		RelationTable<proc_name, proc_name> expected_table;
		expected_table.insert(first, fourth);
		expected_table.insert(first, second);
		expected_table.insert(first, third);
		expected_table.insert(second, fourth);
		expected_table.insert(second, third);
		expected_table.insert(third, fourth);

		PKB::getInstance().addProcedure(first);
		PKB::getInstance().addProcedure(second);
		PKB::getInstance().addProcedure(third);
		PKB::getInstance().addProcedure(fourth);
		PKB::getInstance().addCallsP(first, second);
		PKB::getInstance().addCallsP(second, third);
		PKB::getInstance().addCallsP(third, fourth);
		PKB::getInstance().generateCallsPT();

		EXPECT_EQ(expected_table, PKB::getInstance().getCallsPT());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getCallsPT());
	}
	TEST(PKB, getIf) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		RelationTable<stmt_index, var_name> expected_table;
		expected_table.insert(3, x);
		expected_table.insert(4, y);
		expected_table.insert(4, x);
		expected_table.insert(5, y);

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);

		PKB::getInstance().addVariable(x);
		PKB::getInstance().addIf(3, x);
		EXPECT_THROW(PKB::getInstance().addIf(1, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addIf(4, y);
		PKB::getInstance().addIf(4, x);
		PKB::getInstance().addIf(5, y);
		EXPECT_THROW(PKB::getInstance().addIf(0, y), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addIf(2, y), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addIf(6, y), std::invalid_argument);
		EXPECT_EQ(expected_table, PKB::getInstance().getIf());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getIf());
	}

	TEST(PKB, getWhile) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		RelationTable<stmt_index, var_name> expected_table;
		expected_table.insert(3, x);
		expected_table.insert(4, y);
		expected_table.insert(4, x);
		expected_table.insert(5, y);

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_WHILE);

		PKB::getInstance().addVariable(x);
		PKB::getInstance().addWhile(3, x);
		EXPECT_THROW(PKB::getInstance().addWhile(1, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addWhile(4, y);
		PKB::getInstance().addWhile(4, x);
		PKB::getInstance().addWhile(5, y);
		EXPECT_THROW(PKB::getInstance().addWhile(0, y), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addWhile(2, y), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addWhile(6, y), std::invalid_argument);
		EXPECT_EQ(expected_table, PKB::getInstance().getWhile());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getWhile());
	}

	TEST(PKB, getNext) {
		PKB::getInstance().resetCache();

		RelationTable<StmtInfo, StmtInfo> expected_table;
		StmtInfo s1{ 1, STMT_WHILE };
		StmtInfo s2{ 2, STMT_IF };
		StmtInfo s3{ 3, STMT_PRINT };
		StmtInfo s4{ 4, STMT_ASSIGN };
		StmtInfo s5{ 5, STMT_READ };

		expected_table.insert(s1, s2);
		expected_table.insert(s2, s3);
		expected_table.insert(s2, s4);
		expected_table.insert(s3, s5);
		expected_table.insert(s4, s5);
		expected_table.insert(s5, s1);

		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);

		EXPECT_THROW(PKB::getInstance().addNext(1, 6), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addNext(0, 1), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().addNext(100, 100), std::invalid_argument);
		PKB::getInstance().addNext(1, 2);
		PKB::getInstance().addNext(2, 3);
		PKB::getInstance().addNext(2, 4);
		PKB::getInstance().addNext(3, 5);
		PKB::getInstance().addNext(4, 5);
		PKB::getInstance().addNext(5, 1);

		EXPECT_EQ(expected_table, PKB::getInstance().getNext());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getNext());
	}

	TEST(PKB, getAssigns) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		UniqueRelationTable<stmt_index, var_name> expected_table;
		expected_table.insert(1, x);

		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(2, y);
		EXPECT_EQ(expected_table, PKB::getInstance().getAssigns());
		PKB::getInstance().resetCache();
		EXPECT_NE(expected_table, PKB::getInstance().getAssigns());
	}

	TEST(PKB, getStmt) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		EXPECT_EQ(p1, PKB::getInstance().getStmt(1));
		EXPECT_EQ(p2, PKB::getInstance().getStmt(2));
		EXPECT_THROW(PKB::getInstance().getStmt(0), std::invalid_argument);
		EXPECT_THROW(PKB::getInstance().getStmt(3), std::invalid_argument);
		PKB::getInstance().resetCache();

		EXPECT_THROW(PKB::getInstance().getStmt(1), std::invalid_argument);
	}

	TEST(PKB, getAssignment) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		UniqueRelationTable<stmt_index, var_name> expected_table;
		expected_table.insert(1, x);

		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(2, y);
		EXPECT_EQ(x, PKB::getInstance().getAssignment(1));
		EXPECT_THROW(PKB::getInstance().getAssignment(2), std::invalid_argument);

		PKB::getInstance().resetCache();
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