#include "pch.h"

#include "ResultTable.h"

namespace UnitTesting {
	std::vector<Entity> resultTableHeader1{ {WHILE, "test" }, { READ,"hello" }, {STMT, "test1" }, { IF,"hello3" } };
	std::vector<std::vector<std::string>> resultTableTable1{
		{ "1", "2", "3", "4" },
		{ "5", "6", "7", "8" },
		{ "9", "10", "11", "12" },
		{ "13", "14", "15", "16" },
		{ "17", "18", "19", "20" } };

	std::vector<Entity> resultTableHeader2{ {WHILE, "t1" }, { READ,"t2" } };
	std::vector<std::vector<std::string>> resultTableTable2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::vector<Entity> resultTableHeader3{ {VARIABLE, "t3" }, { IF,"t4" } };
	std::vector<std::vector<std::string>> resultTableTable3{
		{ "6", "16", },
		{ "7", "17", },
		{ "8", "18",},
		{ "9", "19",},
		{ "10", "20", } };

	TEST(ResultTable, init) {
		//TODO
	}

	TEST(ResultTable, merge) {
		//TODO
	}

	TEST(ResultTable, isInTable) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		EXPECT_TRUE(table.isInTable(resultTableHeader1[0]));
		EXPECT_TRUE(table.isInTable(resultTableHeader1[1]));
		EXPECT_TRUE(table.isInTable(resultTableHeader1[2]));
		EXPECT_TRUE(table.isInTable(resultTableHeader1[3]));
		EXPECT_TRUE(table.isInTable({ WHILE, "test" }));
		EXPECT_FALSE(table.isInTable({ PRINT, "test" }));
	}

	TEST(ResultTable, isEmpty) {
		ResultTable table(resultTableHeader1, resultTableTable1);
		EXPECT_FALSE(table.isEmpty());
			
		std::vector<std::vector<std::string>> tables;
		ResultTable table1(resultTableHeader1, tables);
		EXPECT_TRUE(table1.isEmpty());
	}

	TEST(ResultTable, getEntityResult) {
		ResultTable table(resultTableHeader1, resultTableTable1);

		std::list<std::string> result{ "1", "5", "9", "13", "17" };
		std::list<std::string> tableResult = table.getEntityResult(resultTableHeader1[0]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "2", "6", "10", "14", "18" };
		tableResult = table.getEntityResult(resultTableHeader1[1]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "3", "7", "11", "15", "19" };
		tableResult = table.getEntityResult(resultTableHeader1[2]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "4", "8", "12", "16", "20" };
		tableResult = table.getEntityResult(resultTableHeader1[3]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);
	}
}