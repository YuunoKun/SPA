#include "pch.h"

#include "QueryResult.h""

namespace UnitTesting {
	std::pair<Entity, Entity> queryTableHeader1{ {WHILE, Synonym{"test"} }, { READ,Synonym{"hello"} } };
	std::vector<std::pair<std::string, std::string>> queryTableTable1{
		{ "1", "2" }, { "3", "4" },
		{ "5", "6" }, { "7", "8" },
		{ "9", "10" }, { "11", "12" },
		{ "13", "14" }, { "15", "16" },
		{ "17", "18" }, { "19", "20" } };

	std::pair<Entity, Entity> queryTableHeader2{ {WHILE, Synonym{"t1"} }, { READ,Synonym{"t2"} } };
	std::vector<std::pair<std::string, std::string>> queryTableTable2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::pair<Entity, Entity> queryTableHeader3{ {VARIABLE, Synonym{"t3"} }, { IF,Synonym{"t4"} } };
	std::vector<std::pair<std::string, std::string>> queryTableTable3{
		{ "6", "16", },
		{ "7", "17", },
		{ "8", "18",},
		{ "9", "19",},
		{ "10", "20", } };

	TEST(QueryResult, isInTable) {
		ResultTable resultTable1(queryTableHeader1, queryTableTable1);
		ResultTable resultTable2(queryTableHeader2, queryTableTable2);
		ResultTable resultTable3(queryTableHeader3, queryTableTable3);

		QueryResult queryResult;
		queryResult.addResult(resultTable1);
		queryResult.addResult(resultTable2);
		queryResult.addResult(resultTable3);

		EXPECT_TRUE(queryResult.isInTables(queryTableHeader1.first));
		EXPECT_TRUE(queryResult.isInTables(queryTableHeader1.second));
		EXPECT_TRUE(queryResult.isInTables(queryTableHeader2.first));
		EXPECT_TRUE(queryResult.isInTables(queryTableHeader2.second));
		EXPECT_TRUE(queryResult.isInTables(queryTableHeader3.first));
		EXPECT_TRUE(queryResult.isInTables(queryTableHeader3.second));
		EXPECT_TRUE(queryResult.isInTables({ WHILE, Synonym{"test"} }));
		EXPECT_FALSE(queryResult.isInTables({ PRINT,Synonym{ "test" } }));
	}

	TEST(QueryResult, haveResult) {
		QueryResult result;
		EXPECT_TRUE(result.haveResult());

		result.setNoResult();
		EXPECT_FALSE(result.haveResult());

		QueryResult result1;
		Entity e({ WHILE, Synonym{"test"} });
		std::vector<std::string> v;
		result1.addResult({ e, v });
		EXPECT_FALSE(result1.haveResult());

		QueryResult result2;
		Entity e1({ WHILE, Synonym{"test" } });
		std::vector<std::string> v1({ "String" });
		result2.addResult({ e1, v1 });
		EXPECT_TRUE(result2.haveResult());
	}

	TEST(QueryResult, getEntityResult) {
		ResultTable resultTable1(queryTableHeader1, queryTableTable1);
		ResultTable resultTable2(queryTableHeader2, queryTableTable2);
		ResultTable resultTable3(queryTableHeader3, queryTableTable3);

		QueryResult queryResult;
		queryResult.addResult(resultTable1);
		queryResult.addResult(resultTable2);
		queryResult.addResult(resultTable3);

		std::list<std::string> result{ "1", "5", "9", "13", "17", "3", "7", "11", "15", "19" };
		std::list<std::string> tableResult = queryResult.getResult(queryTableHeader1.first);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "2", "6", "10", "14", "18", "4", "8", "12", "16", "20" };
		tableResult = queryResult.getResult(queryTableHeader1.second);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "1", "2", "3", "4", "5" };
		tableResult = queryResult.getResult(queryTableHeader2.first);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "11", "12", "13", "14", "15" };
		tableResult = queryResult.getResult(queryTableHeader2.second);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "6", "7", "8", "9", "10" };
		tableResult = queryResult.getResult(queryTableHeader3.first);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		result = { "16", "17", "18", "19", "20" };
		tableResult = queryResult.getResult(queryTableHeader3.second);
		result.sort();
		tableResult.sort();
		EXPECT_EQ(tableResult, result);

		try {
			tableResult = queryResult.getResult({ STMT, Synonym{"Does not exist"} });
			FAIL();
		}
		catch (std::domain_error const& ex) {
			EXPECT_EQ(ex.what(), std::string("Invalid Entity, Source: QueryResult.getResult"));
		}
	}
}