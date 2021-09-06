#include "pch.h"

#include "ResultTable.h"

namespace UnitTesting {
	std::pair<Entity, Entity> resultTableHeader1{ {WHILE, "test" } , { READ,"hello" } };
	std::vector<std::pair<std::string, std::string>> resultTableTable1{
		{ "1", "2" }, { "3", "4" },
		{ "5", "6" }, { "7", "8" },
		{ "9", "10" }, { "11", "12" },
		{ "13", "14" }, { "15", "16" },
		{ "17", "18" }, { "19", "20" } };

	std::pair<Entity, Entity> resultTableHeader2{ {WHILE, "t1" }, { READ,"t2" } };
	std::vector<std::pair<std::string, std::string>> resultTableTable2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::pair<Entity, Entity> resultTableHeader3{ {VARIABLE, "t3" }, { IF,"t4" } };
	std::vector<std::pair<std::string, std::string>> resultTableTable3{
		{ "6", "16", },
		{ "7", "17", },
		{ "8", "18",},
		{ "9", "19",},
		{ "10", "20", } };

	TEST(ResultTable, initWithString) {
		Entity e(ASSIGN, "x");
		std::vector<std::string> a = { "a", "b", "c", "d", "e" };
		std::list<std::string> b = { "a", "b", "c", "d", "e" };
		ResultTable t(e, a);
		EXPECT_EQ(t.getEntityResult(e), b);
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
		Entity e(ASSIGN, "x");
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		ResultTable t(e, a);
		std::list<std::string> b = { "6", "7" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(WHILE, "x");
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(IF, "x");
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(READ, "x");
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(PRINT, "x");
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e), b);

		a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		e = Entity(CALL, "x");
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e), b);
	}

	TEST(ResultTable, initWithStmtInfoPairStmtInfoAndString) {

		std::pair<Entity, Entity> e = { {ASSIGN, "x"}, {VARIABLE, "y"} };
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };

		ResultTable t(e, a);
		std::list<std::string> b = { "6", "7", "8"};
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "f", "g", "h"};
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = {{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {WHILE, "x"}, {VARIABLE, "y"} };
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "a" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {IF, "x"}, {VARIABLE, "y"} };
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "b" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {READ, "x"}, {VARIABLE, "y"} };
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "c" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {PRINT, "x"}, {VARIABLE, "y"} };
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "d" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };
		e = { {CALL, "x"}, {VARIABLE, "y"} };
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "e" };
		EXPECT_EQ(t.getEntityResult(e.second), b);
	}


	TEST(ResultTable, initWithStmtInfoPairStmtInfoAndStmtInfo) {

		std::pair<Entity, Entity> e = { {ASSIGN, "x"}, {IF, "y"} };
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

		a = {{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {WHILE, "x"}, {READ, "y"} };
		t = ResultTable(e, a);
		b = { "1" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "11" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {IF, "x"}, {IF, "y"} };
		t = ResultTable(e, a);
		b = { "2" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "12" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {READ, "x"}, {CALL, "y"} };
		t = ResultTable(e, a);
		b = { "3" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "13" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {PRINT, "x"}, {IF, "y"} };
		t = ResultTable(e, a);
		b = { "4" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "14" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {CALL, "x"}, {ASSIGN, "y"} };
		t = ResultTable(e, a);
		b = { "5" };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		b = { "15" };
		EXPECT_EQ(t.getEntityResult(e.second), b);

		a = { {{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };
		e = { {WHILE, "x"}, {WHILE, "y"} };
		t = ResultTable(e, a);
		b = { };
		EXPECT_EQ(t.getEntityResult(e.first), b);
		EXPECT_EQ(t.getEntityResult(e.second), b);
	}

	TEST(ResultTable, merge) {
		//TODO
	}

	TEST(ResultTable, isInTable) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		EXPECT_TRUE(table.isInTable(resultTableHeader1.first));
		EXPECT_TRUE(table.isInTable(resultTableHeader1.second));
		EXPECT_TRUE(table.isInTable({ WHILE, "test" }));
		EXPECT_FALSE(table.isInTable({ PRINT, "test" }));
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
}