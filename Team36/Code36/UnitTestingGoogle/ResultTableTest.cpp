#include "pch.h"

#include "ResultTable.h"

namespace UnitTesting {
	std::vector<Entity> header1{ {WHILE, "test" }, { READ,"hello" }, {STMT, "test1" }, { IF,"hello3" } };
	std::vector<std::vector<std::string>> table1{ 
		{ "1", "2", "3", "4" },
		{ "5", "6", "7", "8" },
		{ "9", "10", "11", "12" },
		{ "13", "14", "15", "16" },
		{ "17", "18", "19", "20" } };

	std::vector<Entity> header2{ {WHILE, "t1" }, { READ,"t2" }};
	std::vector<std::vector<std::string>> table2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::vector<Entity> header3{ {VARIABLE, "t3" }, { IF,"t4" } };
	std::vector<std::vector<std::string>> table3{
		{ "5", "16", },
		{ "6", "17", },
		{ "7", "18",},
		{ "8", "19",},
		{ "9", "20", } };

	TEST(ResultTable, merge) {
		//TODO
	}

	TEST(ResultTable, isInTable) {
		ResultTable table(header1, table1);

		EXPECT_TRUE(table.isInTable(header1[0]));
		EXPECT_TRUE(table.isInTable(header1[1]));
		EXPECT_TRUE(table.isInTable(header1[2]));
		EXPECT_TRUE(table.isInTable(header1[3]));
		EXPECT_TRUE(table.isInTable({ WHILE, "test" }));
		EXPECT_FALSE(table.isInTable({ PRINT, "test" }));
	}

	TEST(ResultTable, getEntityResult) {
		ResultTable table(header1, table1);
		
		std::list<std::string> result{ "1", "5", "9", "13", "17" };
		std::list<std::string> tableResult = table.getEntityResult(header1[0]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "2", "6", "10", "14", "18" };
		tableResult = table.getEntityResult(header1[1]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "3", "7", "11", "15", "19" };
		tableResult = table.getEntityResult(header1[2]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "4", "8", "12", "16", "20" };
		tableResult = table.getEntityResult(header1[3]);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);
	}
}