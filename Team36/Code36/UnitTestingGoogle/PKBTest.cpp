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
	TEST(PKB, get_consts) {
		PKB::getInstance().resetCache();

		constant three = 3;
		constant four = 4;
		std::unordered_set<constant> s{ three, four };
		PKB::getInstance().addConstant(three);
		PKB::getInstance().addConstant(four);
		EXPECT_EQ(s, PKB::getInstance().get_consts());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().get_consts());
	}

	TEST(PKB, get_procedures) {
		PKB::getInstance().resetCache();

		proc_name first_procedure = "main";
		proc_name second_procedure = "printY";
		std::vector<proc_name> v{ first_procedure, second_procedure };
		PKB::getInstance().addProcedures(first_procedure);
		PKB::getInstance().addProcedures(second_procedure);
		EXPECT_EQ(v, PKB::getInstance().get_procedures());
		PKB::getInstance().resetCache();
		EXPECT_NE(v, PKB::getInstance().get_procedures());
	}

	TEST(PKB, get_variables) {
		PKB::getInstance().resetCache();

		var_name x = "x";
		var_name y = "y";
		std::vector<var_name> s{ x, y };
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addVariable(y);
		EXPECT_EQ(s, PKB::getInstance().get_variables());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().get_variables());
	}

	TEST(PKB, get_stmts) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		std::vector<StmtInfo> s{ p1, p2, p3 };
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_EQ(s, PKB::getInstance().get_stmts());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().get_stmts());
	}

	TEST(PKB, get_expr_table) {
		PKB::getInstance().resetCache();

		std::unordered_map<var_index, std::string> s{
			{ 1, "x" }, { 2, "x+ y" }, { 3, " y " } };
		PKB::getInstance().addExprTree(1, s[1]);
		PKB::getInstance().addExprTree(2, s[2]);
		PKB::getInstance().addExprTree(3, s[3]);
		EXPECT_EQ(s, PKB::getInstance().get_expr_table());
		PKB::getInstance().resetCache();
		EXPECT_NE(s, PKB::getInstance().get_expr_table());
	}

	TEST(PKB, get_parent_table) {
		PKB::getInstance().resetCache();

<<<<<<< HEAD
		proc_name first_procedure = "main";
		proc_name second_procedure = "printY";
		std::vector<proc_name> v{ first_procedure, second_procedure };
		PKB::getInstance().setProcedures(v);
		EXPECT_EQ(v, PKB::getInstance().getProcedures());
=======
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		RelationTable<StmtInfo, StmtInfo> table;
		table.insert(p1, p2);
		table.insert(p1, p3);
		table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		EXPECT_EQ(table, PKB::getInstance().get_parent_table());
		PKB::getInstance().resetCache();
		EXPECT_NE(table, PKB::getInstance().get_parent_table());
>>>>>>> br-develop
	}

	TEST(PKB, get_follows_table) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		UniqueRelationTable<StmtInfo, StmtInfo> table;
		table.insert(p1, p2);
		table.insert(p2, p3);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(1, 3);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_EQ(table, PKB::getInstance().get_follows_table());
		PKB::getInstance().resetCache();
		EXPECT_NE(table, PKB::getInstance().get_follows_table());
	}

	TEST(PKB, get_usesS_table) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> table;
		table.insert(p2, x);
		table.insert(p4, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addUsesS(2, x);
		EXPECT_THROW(PKB::getInstance().addUsesS(4, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addUsesS(4, y);
		EXPECT_EQ(table, PKB::getInstance().get_usesS_table());
		PKB::getInstance().resetCache();
		EXPECT_NE(table, PKB::getInstance().get_usesS_table());
	}

	TEST(PKB, get_modifiesS_table) {
		PKB::getInstance().resetCache();

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_PRINT };
		var_name x = "x";
		var_name y = "y";
		RelationTable<StmtInfo, var_name> table;
		table.insert(p1, x);
		table.insert(p3, y);

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addVariable(x);
		PKB::getInstance().addModifiesS(1, x);
		EXPECT_THROW(PKB::getInstance().addModifiesS(3, y), std::invalid_argument);
		PKB::getInstance().addVariable(y);
		PKB::getInstance().addModifiesS(3, y);
		EXPECT_EQ(table, PKB::getInstance().get_modifiesS_table());
		PKB::getInstance().resetCache();
		EXPECT_NE(table, PKB::getInstance().get_modifiesS_table());
	}

	TEST(PKB, resetCache) {
		PKB::getInstance().resetCache();

		std::vector<constant> vc{ 3 };
		std::vector<proc_name> vp{ "main" };
		PKB::getInstance().setConstants(vc);
		PKB::getInstance().setProcedures(vp);
		PKB::getInstance().resetCache();
		EXPECT_EQ(PKB::getInstance().getConstants().size(), 0);
		EXPECT_EQ(PKB::getInstance().getProcedures().size(), 0);
	}
}