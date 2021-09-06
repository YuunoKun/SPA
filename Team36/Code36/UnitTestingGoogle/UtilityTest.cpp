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
		std::vector<proc_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		EXPECT_EQ(Utility::proceduresToStringList(a), b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(Utility::proceduresToStringList(a), c);

		std::vector<proc_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		EXPECT_EQ(Utility::proceduresToStringList(d), e);
	}

	TEST(Utility, stmtInfoToStringList) {
		std::vector<StmtInfo> a{ {1, STMT_ASSIGN}, {2, STMT_CALL} ,
			{3, STMT_IF} ,{4, STMT_WHILE} ,{5, STMT_PRINT} };
		std::list<std::string> b{ "1", "2", "3", "4", "5" };

		EXPECT_EQ(Utility::stmtInfoToStringList(a), b);

		std::list<std::string> c{ "1", "2", "3", "4" };
		EXPECT_NE(Utility::stmtInfoToStringList(a), c);

		std::vector<StmtInfo> d{ {1, STMT_ASSIGN}, {2, STMT_CALL} };
		std::list<std::string> e{ "1", "2" };

		EXPECT_EQ(Utility::stmtInfoToStringList(d), e);
	}

	TEST(Utility, variablesToStringList) {
		std::vector<var_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		EXPECT_EQ(Utility::variablesToStringList(a), b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(Utility::variablesToStringList(a), c);

		std::vector<var_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		EXPECT_EQ(Utility::variablesToStringList(d), e);
	}

	TEST(Utility, unorderedSetToStringList) {
		std::unordered_set<std::string> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		EXPECT_EQ(Utility::unorderedSetToStringList(a), b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(Utility::unorderedSetToStringList(a), c);

		std::unordered_set<std::string> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		EXPECT_EQ(Utility::unorderedSetToStringList(d), e);
	}

	TEST(Utility, stmtToStmtInfoVector) {
		std::vector<Stmt> a = { { STMT_ASSIGN, 1 }, { STMT_CALL, 2} , { STMT_WHILE, 3 }, { STMT_IF, 4} };
		std::vector<StmtInfo> b = { { 1, STMT_ASSIGN }, { 2, STMT_CALL} , { 3, STMT_WHILE }, { 4, STMT_IF} };

		EXPECT_EQ(Utility::stmtToStmtInfoVector(a), b);
	}

	TEST(Utility, stmtInfoToStringVector) {
		std::vector<StmtInfo> a = { { 1, STMT_ASSIGN }, { 2, STMT_CALL} , { 3, STMT_WHILE }, { 4, STMT_IF} };
		std::vector<std::string> b{ "1", "2", "3", "4" };
		EXPECT_EQ(Utility::stmtInfoToStringVector(a), b);
	}

	TEST(Utility, pairToStringTable) {
		std::vector<std::pair<std::string, std::string>> a = { {"a", "b"},{"c", "d"} };
		std::vector<std::vector<std::string>> b = { {"a", "b"},{"c", "d"} };
		EXPECT_EQ(Utility::pairToStringTable(a), b);

		std::vector<std::pair<StmtInfo, std::string>> c = { {{ 1, STMT_ASSIGN }, "b"},{{ 2, STMT_ASSIGN }, "d"} };
		std::vector<std::vector<std::string>> d = { {"1", "b"},{"2", "d"} };
		EXPECT_EQ(Utility::pairToStringTable(c), d);

		std::vector<std::pair<StmtInfo, StmtInfo>> e = { {{ 1, STMT_ASSIGN }, { 3, STMT_WHILE }},{{ 2, STMT_ASSIGN }, { 4, STMT_WHILE }} };
		std::vector<std::vector<std::string>> f = { {"1", "3"},{"2", "4"} };
		EXPECT_EQ(Utility::pairToStringTable(e), f);
	}
	TEST(Utility, getIndexString) {
		std::vector<std::string> c{ "a", "b", "c", "d", "e" };
		EXPECT_EQ(Utility::getIndex(c, c[0]), 0);
		EXPECT_EQ(Utility::getIndex(c, c[1]), 1);
		EXPECT_EQ(Utility::getIndex(c, c[2]), 2);
		EXPECT_EQ(Utility::getIndex(c, c[3]), 3);
		EXPECT_EQ(Utility::getIndex(c, c[4]), 4);
	}

	TEST(Utility, getIndexEntity) {
		std::vector<Entity> e{ {WHILE, "test" }, { READ,"hello" }, { PRINT,"test1" }, { STMT,"test3" } };

		EXPECT_EQ(Utility::getIndex(e, e[0]), 0);
		EXPECT_EQ(Utility::getIndex(e, e[1]), 1);
		EXPECT_EQ(Utility::getIndex(e, e[2]), 2);
		EXPECT_EQ(Utility::getIndex(e, e[3]), 3);
	}

	TEST(Utility, convertType) {
		EntityType a = WHILE;
		EXPECT_EQ(Utility::convertType(a), STMT_WHILE);

		a = IF;
		EXPECT_EQ(Utility::convertType(a), STMT_IF);

		a = READ;
		EXPECT_EQ(Utility::convertType(a), STMT_READ);

		a = PRINT;
		EXPECT_EQ(Utility::convertType(a), STMT_PRINT);

		a = CALL;
		EXPECT_EQ(Utility::convertType(a), STMT_CALL);

		a = ASSIGN;
		EXPECT_EQ(Utility::convertType(a), STMT_ASSIGN);
	}

	TEST(Utility, filterResult) {
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ }, { 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::vector<StmtInfo> b = { { 6, STMT_ASSIGN } , { 7, STMT_ASSIGN } };
		EXPECT_EQ(Utility::filterResult(ASSIGN, a), b);

		b = { { 1,  STMT_WHILE} };
		EXPECT_EQ(Utility::filterResult(WHILE, a), b);

		b = { { 2,  STMT_IF} };
		EXPECT_EQ(Utility::filterResult(IF, a), b);

		b = { { 3,  STMT_READ} };
		EXPECT_EQ(Utility::filterResult(READ, a), b);

		b = { { 4,  STMT_PRINT} };
		EXPECT_EQ(Utility::filterResult(PRINT, a), b);

		b = { { 5,  STMT_CALL} };
		EXPECT_EQ(Utility::filterResult(CALL, a), b);
	}

	TEST(Utility, filterResults) {
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };

		std::vector<std::vector<std::string>> b = { {"6", "f"}, {"7", "g"}, { "8", "h"} };
		EXPECT_EQ(Utility::filterResults(ASSIGN, a), b);

		b = { {"1", "a"} };
		EXPECT_EQ(Utility::filterResults(WHILE, a), b);

		b = { {"2", "b"} };
		EXPECT_EQ(Utility::filterResults(IF, a), b);

		b = { {"3", "c"} };
		EXPECT_EQ(Utility::filterResults(READ, a), b);

		b = { {"4", "d"} };
		EXPECT_EQ(Utility::filterResults(PRINT, a), b);

		b = { {"5", "e"} };
		EXPECT_EQ(Utility::filterResults(CALL, a), b);

		std::vector<std::pair<StmtInfo, StmtInfo>> c = {
			{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };

		std::vector<std::vector<std::string>> d = { {"6", "16"}, {"7", "17"}, { "8", "18"} };
		EXPECT_EQ(Utility::filterResults({ ASSIGN, IF }, c), d);

		d = { {"1", "11"} };
		EXPECT_EQ(Utility::filterResults({ WHILE, READ }, c), d);

		d = { {"2", "12"} };
		EXPECT_EQ(Utility::filterResults({ IF, IF }, c), d);

		d = { {"3", "13"} };
		EXPECT_EQ(Utility::filterResults({ READ, CALL }, c), d);

		d = { {"4", "14"} };
		EXPECT_EQ(Utility::filterResults({ PRINT, IF }, c), d);

		d = { {"5", "15"} };
		EXPECT_EQ(Utility::filterResults({ CALL, ASSIGN }, c), d);

		d = { };

		EXPECT_EQ(Utility::filterResults({ WHILE, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ WHILE, CALL }, c), d);
		EXPECT_EQ(Utility::filterResults({ WHILE, IF }, c), d);
		EXPECT_EQ(Utility::filterResults({ WHILE, PRINT }, c), d);
		EXPECT_EQ(Utility::filterResults({ WHILE, ASSIGN }, c), d);

		EXPECT_EQ(Utility::filterResults({ READ, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ READ, READ }, c), d);
		EXPECT_EQ(Utility::filterResults({ READ, IF }, c), d);
		EXPECT_EQ(Utility::filterResults({ READ, PRINT }, c), d);
		EXPECT_EQ(Utility::filterResults({ READ, ASSIGN }, c), d);

		EXPECT_EQ(Utility::filterResults({ CALL, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ CALL, READ }, c), d);
		EXPECT_EQ(Utility::filterResults({ CALL, CALL }, c), d);
		EXPECT_EQ(Utility::filterResults({ CALL, IF }, c), d);
		EXPECT_EQ(Utility::filterResults({ CALL, PRINT }, c), d);

		EXPECT_EQ(Utility::filterResults({ IF, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ IF, READ }, c), d);
		EXPECT_EQ(Utility::filterResults({ IF, CALL }, c), d);
		EXPECT_EQ(Utility::filterResults({ IF, PRINT }, c), d);
		EXPECT_EQ(Utility::filterResults({ IF, ASSIGN }, c), d);

		EXPECT_EQ(Utility::filterResults({ PRINT, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ PRINT, READ }, c), d);
		EXPECT_EQ(Utility::filterResults({ PRINT, CALL }, c), d);
		EXPECT_EQ(Utility::filterResults({ PRINT, PRINT }, c), d);
		EXPECT_EQ(Utility::filterResults({ PRINT, ASSIGN }, c), d);

		EXPECT_EQ(Utility::filterResults({ ASSIGN, WHILE }, c), d);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, READ }, c), d);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, CALL }, c), d);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, PRINT }, c), d);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, ASSIGN }, c), d);
	}
}