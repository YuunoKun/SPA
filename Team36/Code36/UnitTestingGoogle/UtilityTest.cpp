#include "pch.h"

#include "Utility.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Utility, constantsToStringList) {
		std::vector<constant> a{ 1, 2, 3, 4, 5 };
		std::list<std::string> b{ "1", "2", "3", "4", "5" };

		EXPECT_EQ(Utility::constantsToStringList(a), b);

		std::list<std::string> c{ "1", "2", "3", "4" };
		EXPECT_NE(Utility::constantsToStringList(a), c);

		std::vector<constant> d{ 1, 2 };
		std::list<std::string> e{ "1", "2" };

		EXPECT_EQ(Utility::constantsToStringList(d), e);
	}

	TEST(Utility, proceduresToStringList) {
		std::vector<procedure_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		EXPECT_EQ(Utility::proceduresToStringList(a), b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(Utility::proceduresToStringList(a), c);

		std::vector<procedure_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		EXPECT_EQ(Utility::proceduresToStringList(d), e);
	}

	TEST(Utility, stmtsToStringList) {
		std::vector<Stmt> a{ {STMT_ASSIGN,1}, {STMT_CALL,2} ,
			{STMT_IF,3} ,{STMT_WHILE,4} ,{STMT_PRINT,5} };
		std::list<std::string> b{ "1", "2", "3", "4", "5" };

		EXPECT_EQ(Utility::stmtsToStringList(a), b);


		std::list<std::string> c{ "1", "2", "3", "4" };
		EXPECT_NE(Utility::stmtsToStringList(a), c);

		std::vector<Stmt> d{ {STMT_ASSIGN,1}, {STMT_CALL,2} };
		std::list<std::string> e{ "1", "2" };

		EXPECT_EQ(Utility::stmtsToStringList(d), e);
	}

	TEST(Utility, variablesToStringList) {
		std::vector<variable_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		EXPECT_EQ(Utility::variablesToStringList(a), b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(Utility::variablesToStringList(a), c);

		std::vector<variable_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		EXPECT_EQ(Utility::variablesToStringList(d), e);
	}
}