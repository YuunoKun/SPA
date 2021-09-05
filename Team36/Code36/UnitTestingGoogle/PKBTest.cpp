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