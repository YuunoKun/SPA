#include "pch.h"

#include "ResultTable.h"

namespace UnitTesting {
	std::pair<Entity, Entity> resultTableHeader1{ {WHILE, Synonym{"test"} } , { READ,Synonym{"hello"} } };
	std::vector<std::pair<std::string, std::string>> resultTableTable1{
		{ "1", "2" }, { "3", "4" },
		{ "5", "6" }, { "7", "8" },
		{ "9", "10" }, { "11", "12" },
		{ "13", "14" }, { "15", "16" },
		{ "17", "18" }, { "19", "20" } };

	std::pair<Entity, Entity> resultTableHeader2{ {WHILE, Synonym{"t1"} }, { READ,Synonym{"t2"} } };
	std::vector<std::pair<std::string, std::string>> resultTableTable2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::pair<Entity, Entity> resultTableHeader3{ {VARIABLE, Synonym{"t3"} }, { IF,Synonym{"t4"} } };
	std::vector<std::pair<std::string, std::string>> resultTableTable3{
		{ "6", "16", },
		{ "7", "17", },
		{ "8", "18",},
		{ "9", "19",},
		{ "10", "20", } };

	TEST(ResultTable, initWithString) {
		Entity e(ASSIGN, Synonym{ "x" });
		std::vector<std::string> a = { "a", "b", "c", "d", "e" };
		std::list<std::string> b = { "a", "b", "c", "d", "e" };
		ResultTable t(e, a);
		EXPECT_EQ(t.getEntityResult(e), b);
	}

	TEST(ResultTable, initWithStmtIndex) {
		Entity e(ASSIGN, Synonym{ "x" });
		std::vector<stmt_index> a = { 1, 2, 3, 4, 5 };
		std::list<std::string> b = { "1", "2", "3", "4", "5" };
		ResultTable t(e, a);
		EXPECT_EQ(t.getEntityResult(e), b);
	}

	TEST(ResultTable, initWithPairStmtIndexString) {
		Entity e1(ASSIGN, Synonym{ "x" });
		Entity e2(VARIABLE, Synonym{ "y" });
		std::vector<std::pair<stmt_index, std::string>> a = {
			{ 1 , "a"}, {2, "b"} ,
			{ 3, "c"}, {4, "d"} ,
			{ 5 , "e"} };
		std::list<std::string> b1 = { "1", "2", "3", "4", "5" };
		std::list<std::string> b2 = { "a", "b", "c", "d", "e" };
		ResultTable t({ e1, e2 }, a);
		EXPECT_EQ(t.getEntityResult(e1), b1);
		EXPECT_EQ(t.getEntityResult(e2), b2);
	}
	TEST(ResultTable, initWithPairString) {
		ResultTable table(resultTableHeader1, resultTableTable1);
		std::list<std::string> result{ "1", "5", "9", "13", "17", "3", "7", "11", "15", "19" };
		std::list<std::string> tableResult = table.getEntityResult(resultTableHeader1.first);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "2", "6", "10", "14", "18", "4", "8", "12", "16", "20" };
		tableResult = table.getEntityResult(resultTableHeader1.second);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);
	}
	TEST(ResultTable, initWithStmtInfo) {
		Entity e(ASSIGN, Synonym{ "x" });
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		ResultTable t(e, a);
		std::list<std::string> b = { "6", "7" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(WHILE, Synonym{ "x" });
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(IF, Synonym{ "x" });
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(READ, Synonym{ "x" });
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(PRINT, Synonym{ "x" });
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(CALL, Synonym{ "x" });
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e), b);
	}

	TEST(ResultTable, initWithStmtInfoPairStmtInfoAndString) {
		std::pair<Entity, Entity> e = { {ASSIGN, Synonym{"x"}}, {VARIABLE,Synonym{ "y"}} };
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };

		ResultTable t(e, a);
		std::list<std::string> b = { "6", "7", "8" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "f", "g", "h" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {WHILE,Synonym{ "x"}}, {VARIABLE, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "a" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {IF,Synonym {"x"}}, {VARIABLE, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "b" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {READ,Synonym{ "x"}}, {VARIABLE,Synonym{ "y"}} };
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "c" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {PRINT, Synonym{"x"}}, {VARIABLE,Synonym{ "y"}} };
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "d" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {CALL, Synonym{"x"}}, {VARIABLE, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "e" };
		EXPECT_EQ(t.getEntityResult(e.second), b);
	}

	TEST(ResultTable, initWithStmtInfoPairStmtInfoAndStmtInfo) {
		std::pair<Entity, Entity> e = { {ASSIGN, Synonym{"x"}}, {IF, Synonym{"y"}} };
		std::vector<std::pair<StmtInfo, StmtInfo>> a = {
			{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };

		ResultTable t(e, a);
		std::list<std::string> b = { "6", "7", "8" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "16", "17", "18" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {WHILE, Synonym{"x"}}, {READ,Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "11" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {IF, Synonym{"x"}}, {IF, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "12" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {READ, Synonym{"x"}}, {CALL, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "13" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {PRINT,Synonym{ "x"}}, {IF, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "14" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {CALL, Synonym{"x"}}, {ASSIGN, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "15" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {WHILE, Synonym{"x"}}, {WHILE, Synonym{"y"}} };
		t = ResultTable(e, a);
		b = { };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		EXPECT_EQ(t.getEntityResult(e.second), b);
	}

	TEST(ResultTable, mergeFail) {
		Entity e1(STMT, Synonym{ "x" });
		Entity e2(STMT, Synonym{ "y" });
		std::vector<StmtInfo> a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::vector<StmtInfo> a2 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		ResultTable t1(e1, a1);
		ResultTable t2(e2, a2);
		EXPECT_FALSE(t1.merge(t2));
	}
	TEST(ResultTable, mergeFilterSingleColumnSingleTable) {
		Entity e(STMT, Synonym{ "x" });
		std::vector<StmtInfo> a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::vector<StmtInfo> a2 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		ResultTable t1(e, a1);
		ResultTable t2(e, a2);
		EXPECT_TRUE(t1.merge(t2));
		std::list<std::string> b = { "1", "3", "5", "7" };
		EXPECT_EQ(t1.getEntityResult(e), b);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e), b);

		a1 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		a2 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		t1.merge(t2);
		EXPECT_EQ(t1.getEntityResult(e), b);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e), b);

		a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		a2 = { { 8, STMT_WHILE } , { 9, STMT_READ }, { 10, STMT_CALL }, { 11, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		t1.merge(t2);
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());

		a1 = { { 8, STMT_WHILE } , { 9, STMT_READ }, { 10, STMT_CALL }, { 11, STMT_ASSIGN} };
		a2 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
	}

	TEST(ResultTable, mergeFilterSingleColumnDoubleTable) {
		std::pair<Entity, Entity> e1{ {STMT, Synonym{"x"} } , { STMT,Synonym{"y"} } };
		Entity e2(STMT, Synonym{ "x" });
		std::vector<std::pair<std::string, std::string>> a1{
			{ "1", "2", },
			{ "3", "4", },
			{ "5", "6", },
			{ "7", "8", },
			{ "9", "10", }
		};
		std::vector<StmtInfo> a2 = { { 1, STMT_WHILE }, { 5, STMT_CALL }, { 9, STMT_ASSIGN} };

		ResultTable t1(e1, a1);
		ResultTable t2(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		std::list<std::string> b1 = { "9", "1", "5" };
		std::list<std::string> b2 = { "2", "6", "10" };
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		e2 = { STMT, Synonym{ "y" } };
		a2 = { { 4, STMT_WHILE }, { 8, STMT_CALL } };

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		b1 = { "3", "7" };
		b2 = { "4", "8" };
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		a2 = { { 4, STMT_WHILE }, { 8, STMT_CALL } };

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		e1 = { {STMT, Synonym{"x"} } , { STMT,Synonym{"y"} } };
		e2 = { STMT, Synonym{ "x" } };
		a1 = {
			{ "1", "2", },
			{ "3", "4", },
			{ "5", "6", },
			{ "7", "8", },
			{ "9", "10", }
		};
		a2 = { { 11, STMT_WHILE }, { 555, STMT_CALL }, { 99, STMT_ASSIGN} };

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t2.isEmpty());
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t2.isEmpty());

		e2 = { STMT, Synonym{ "y" } };

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t2.isEmpty());
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t2.isEmpty());
	}
	TEST(ResultTable, mergeFilterDoubleColumnDoubleTable) {
		std::pair<Entity, Entity> e1{ {STMT, Synonym{"x"} } , { STMT,Synonym{"y"} } };
		std::pair<Entity, Entity> e2{ {STMT, Synonym{"x"} } , { STMT,Synonym{"y"} } };
		std::vector<std::pair<std::string, std::string>> a1{
			{ "1", "2", },
			{ "3", "4", },
			{ "5", "6", },
			{ "7", "8", },
			{ "9", "10", }
		};
		std::vector<std::pair<std::string, std::string>> a2{
			{ "1", "2", },
			{ "5", "6", },
			{ "9", "10", }
		};

		ResultTable t1(e1, a1);
		ResultTable t2(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		std::list<std::string> b1 = { "9", "1", "5" };
		std::list<std::string> b2 = { "2", "6", "10" };
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		e2 = { {STMT, Synonym{"y"} } , { STMT,Synonym{"x"} } };
		a2 = {
			{ "2", "1", },
			{ "6", "5", },
			{ "10", "9", }
		};

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		b1 = { "9", "1", "5" };
		b2 = { "2", "6", "10" };
		EXPECT_EQ(t1.getEntityResult(e1.first), b1);
		EXPECT_EQ(t1.getEntityResult(e1.second), b2);

		e2 = { {STMT, Synonym{"y"} } , { STMT,Synonym{"x"} } };
		a2 = {
			{ "2", "2", },
			{ "6", "2", },
			{ "10", "2", }
		};

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.isEmpty());
	}

	TEST(ResultTable, mergeFilterDoubleColumnTripleTable) {
		Entity e1 = { STMT, Synonym{"x"} };
		Entity e2 = { STMT, Synonym{"y"} };
		Entity e3 = { STMT, Synonym{"z"} };
		std::pair<Entity, Entity> h1{ e1, e2 };
		std::pair<Entity, Entity> h2{ e2, e3 };
		std::pair<Entity, Entity> h3{ e1, e3 };
		std::vector<std::pair<std::string, std::string>> a1{
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		std::vector<std::pair<std::string, std::string>> a2{
			{ "4", "7", },
			{ "5", "8", },
			{ "6", "9", }
		};
		std::vector<std::pair<std::string, std::string>> a3{
			{ "1", "7", },
			{ "2", "8", }
		};

		ResultTable t1(h1, a1);
		ResultTable t2(h2, a2);
		ResultTable t3(h3, a3);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.merge(t3));
		std::list<std::string> b1 = { "1", "2" };
		std::list<std::string> b2 = { "4", "5" };
		std::list<std::string> b3 = { "7", "8" };
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t3.merge(t1));
		EXPECT_EQ(t3.getEntityResult(e1), b1);
		EXPECT_EQ(t3.getEntityResult(e2), b2);
		EXPECT_EQ(t3.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t1.merge(t3));
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t1.merge(t3));
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_EQ(t2.getEntityResult(e1), b1);
		EXPECT_EQ(t2.getEntityResult(e2), b2);
		EXPECT_EQ(t2.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t2.merge(t3));
		EXPECT_EQ(t2.getEntityResult(e1), b1);
		EXPECT_EQ(t2.getEntityResult(e2), b2);
		EXPECT_EQ(t2.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_TRUE(t3.merge(t2));
		EXPECT_EQ(t3.getEntityResult(e1), b1);
		EXPECT_EQ(t3.getEntityResult(e2), b2);
		EXPECT_EQ(t3.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t2.merge(t3));
		EXPECT_TRUE(t2.merge(t1));
		EXPECT_EQ(t2.getEntityResult(e1), b1);
		EXPECT_EQ(t2.getEntityResult(e2), b2);
		EXPECT_EQ(t2.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t2.merge(t3));
		EXPECT_TRUE(t1.merge(t3));
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t3.merge(t1));
		EXPECT_TRUE(t3.merge(t2));
		EXPECT_EQ(t3.getEntityResult(e1), b1);
		EXPECT_EQ(t3.getEntityResult(e2), b2);
		EXPECT_EQ(t3.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t3.merge(t1));
		EXPECT_TRUE(t2.merge(t3));
		EXPECT_EQ(t2.getEntityResult(e1), b1);
		EXPECT_EQ(t2.getEntityResult(e2), b2);
		EXPECT_EQ(t2.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t3.merge(t2));
		EXPECT_TRUE(t3.merge(t1));
		EXPECT_EQ(t3.getEntityResult(e1), b1);
		EXPECT_EQ(t3.getEntityResult(e2), b2);
		EXPECT_EQ(t3.getEntityResult(e3), b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		EXPECT_TRUE(t3.merge(t2));
		EXPECT_TRUE(t1.merge(t3));
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		h1 = { e1, e2 };
		h2 = { e3, e2 };
		h3 = { e1, e2 };
		a1 = {
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		a2 = {
			{ "7", "4", },
			{ "8", "5", },
			{ "9", "6", }
		};
		a3 = {
			{ "4", "7", },
			{ "5", "9", },
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t2 = ResultTable(h3, a3);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_TRUE(t1.merge(t3));
		EXPECT_TRUE(t1.isEmpty());
	}

	TEST(ResultTable, mergeJoinTableSingleColumn) {
		Entity e1 = { STMT, Synonym{"x"} };
		Entity e2 = { STMT, Synonym{"y"} };
		Entity e3 = { STMT, Synonym{"z"} };
		std::pair<Entity, Entity> h1{ e1, e2 };
		std::pair<Entity, Entity> h2{ e2, e3 };
		std::vector<std::pair<std::string, std::string>> a1{
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		std::vector<std::pair<std::string, std::string>> a2{
			{ "4", "7", },
			{ "5", "8", },
			{ "6", "9", }
		};

		ResultTable t1(h1, a1);
		ResultTable t2(h2, a2);
		EXPECT_TRUE(t1.merge(t2));
		std::list<std::string> b1 = { "1", "2", "3" };
		std::list<std::string> b2 = { "4", "5", "6" };
		std::list<std::string> b3 = { "7", "8", "9" };
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		h1 = { e1, e2 };
		h2 = { e3, e2 };
		a1 = {
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		a2 = {
			{ "7", "4", },
			{ "8", "5", },
			{ "9", "6", }
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);

		h1 = { e1, e2 };
		h2 = { e1, e3 };
		a1 = {
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		a2 = {
			{ "1", "7", },
			{ "2", "8", },
			{ "3", "9", }
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		EXPECT_TRUE(t1.merge(t2));
		EXPECT_EQ(t1.getEntityResult(e1), b1);
		EXPECT_EQ(t1.getEntityResult(e2), b2);
		EXPECT_EQ(t1.getEntityResult(e3), b3);
	}
	TEST(ResultTable, isInTable) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		EXPECT_TRUE(table.isInTable(resultTableHeader1.first));
		EXPECT_TRUE(table.isInTable(resultTableHeader1.second));
		EXPECT_TRUE(table.isInTable({ WHILE, Synonym{"test"} }));
		EXPECT_FALSE(table.isInTable({ PRINT, Synonym{"test1"} }));
	}

	TEST(ResultTable, isEmpty) {
		ResultTable table(resultTableHeader1, resultTableTable1);
		EXPECT_FALSE(table.isEmpty());

		std::vector<std::pair<std::string, std::string>> tables;
		ResultTable table1(resultTableHeader1, tables);
		EXPECT_TRUE(table1.isEmpty());
	}

	TEST(ResultTable, getEntityResult) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		std::list<std::string> result{ "1", "5", "9", "13", "17", "3", "7", "11", "15", "19" };
		std::list<std::string> tableResult = table.getEntityResult(resultTableHeader1.first);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "2", "6", "10", "14", "18", "4", "8", "12", "16", "20" };
		tableResult = table.getEntityResult(resultTableHeader1.second);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);
	}

	TEST(ResultTable, getCommonHeaders) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		std::vector<Entity> v = { resultTableHeader1.first };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.first }), v);

		v = { resultTableHeader1.second };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.second }), v);

		v = { resultTableHeader1.first, resultTableHeader1.second };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.first, resultTableHeader1.second }), v);

		v = { resultTableHeader1.first, resultTableHeader1.second };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.first, resultTableHeader1.second, resultTableHeader2.first, resultTableHeader2.second }), v);

		v = { resultTableHeader1.first };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.first, resultTableHeader2.first, resultTableHeader2.second }), v);

		v = { resultTableHeader1.second };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader1.second, resultTableHeader2.first, resultTableHeader2.second }), v);

		v = { };
		EXPECT_EQ(table.getCommonHeaders({ resultTableHeader2.first, resultTableHeader2.second }), v);
	}
}