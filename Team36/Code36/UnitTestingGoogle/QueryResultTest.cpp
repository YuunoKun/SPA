#include "pch.h"

#include "QueryResult.h""

namespace UnitTesting {
	std::pair<Entity, Entity> query_table_header1{ {WHILE, Synonym{"test"} }, { READ,Synonym{"hello"} } };
	std::vector<std::pair<std::string, std::string>> query_list_pair1{
		{ "1", "2" }, { "3", "4" },
		{ "5", "6" }, { "7", "8" },
		{ "9", "10" }, { "11", "12" },
		{ "13", "14" }, { "15", "16" },
		{ "17", "18" }, { "19", "20" } };

	std::pair<Entity, Entity> query_table_header2{ {WHILE, Synonym{"t1"} }, { READ,Synonym{"t2"} } };
	std::vector<std::pair<std::string, std::string>> query_list_pair2{
		{ "1", "11", },
		{ "2", "12", },
		{ "3", "13",},
		{ "4", "14",},
		{ "5", "15", } };

	std::pair<Entity, Entity> query_table_header3{ {VARIABLE, Synonym{"t3"} }, { IF,Synonym{"t4"} } };
	std::vector<std::pair<std::string, std::string>> query_list_pair3{
		{ "6", "16", },
		{ "7", "17", },
		{ "8", "18",},
		{ "9", "19",},
		{ "10", "20", } };

	TEST(QueryResult, isInTable) {
		ResultTable result_table1(query_table_header1, query_list_pair1);
		ResultTable result_table2(query_table_header2, query_list_pair2);
		ResultTable result_table3(query_table_header3, query_list_pair3);

		QueryResult query_result;
		query_result.addResult(result_table1);
		query_result.addResult(result_table2);
		query_result.addResult(result_table3);

		EXPECT_TRUE(query_result.isInTables(query_table_header1.first));
		EXPECT_TRUE(query_result.isInTables(query_table_header1.second));
		EXPECT_TRUE(query_result.isInTables(query_table_header2.first));
		EXPECT_TRUE(query_result.isInTables(query_table_header2.second));
		EXPECT_TRUE(query_result.isInTables(query_table_header3.first));
		EXPECT_TRUE(query_result.isInTables(query_table_header3.second));
		EXPECT_TRUE(query_result.isInTables({ WHILE, Synonym{"test"} }));
		EXPECT_FALSE(query_result.isInTables({ PRINT,Synonym{ "test1" } }));
	}

	TEST(QueryResult, haveResult) {
		QueryResult result;
		EXPECT_TRUE(result.haveResult());

		result.setNoResult();
		EXPECT_FALSE(result.haveResult());

		QueryResult result1;
		Entity e({ WHILE, Synonym{"test"} });
		std::vector<std::string> v;

		result1.addResult(ResultTable(e, v));
		EXPECT_FALSE(result1.haveResult());

		QueryResult result2;
		Entity e1({ WHILE, Synonym{"test" } });
		std::vector<std::string> v1({ "String" });
		result2.addResult(ResultTable(e1, v1));
		EXPECT_TRUE(result2.haveResult());
	}

	TEST(QueryResult, getEntityResult) {
		ResultTable result_table1(query_table_header1, query_list_pair1);
		ResultTable result_table2(query_table_header2, query_list_pair2);
		ResultTable result_table3(query_table_header3, query_list_pair3);

		QueryResult query_result;
		query_result.addResult(result_table1);
		query_result.addResult(result_table2);
		query_result.addResult(result_table3);

		std::list<std::string> result{ "1", "5", "9", "13", "17", "3", "7", "11", "15", "19" };
		std::list<std::string> table_result;
		query_result.getResult(query_table_header1.first, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		result = { "2", "6", "10", "14", "18", "4", "8", "12", "16", "20" };
		table_result.clear();
		query_result.getResult(query_table_header1.second, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		result = { "1", "2", "3", "4", "5" };

		table_result.clear();
		query_result.getResult(query_table_header2.first, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		result = { "11", "12", "13", "14", "15" };

		table_result.clear();
		query_result.getResult(query_table_header2.second, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		result = { "6", "7", "8", "9", "10" };

		table_result.clear();
		query_result.getResult(query_table_header3.first, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		result = { "16", "17", "18", "19", "20" };

		table_result.clear();
		query_result.getResult(query_table_header3.second, table_result);
		result.sort();
		table_result.sort();
		EXPECT_EQ(table_result, result);

		try {
			query_result.getResult({ STMT, Synonym{"DoesNotExist"} }, table_result);
			FAIL();
		} catch (std::domain_error const& ex) {
			EXPECT_EQ(ex.what(), std::string("Invalid Entity, Source: QueryResult.getResult"));
		}
	}

	TEST(QueryResult, addResultNoMerge) {
		Entity e1(STMT, Synonym{ "x" });
		Entity e2(STMT, Synonym{ "y" });
		std::vector<StmtInfo> a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::vector<StmtInfo> a2 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		ResultTable t1(e1, a1);
		ResultTable t2(e2, a2);

		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		std::list<std::string> b1 = { "1", "2", "3", "4", "5", "6", "7" };
		std::list<std::string> b2 = { "1", "3", "5", "7" };

		std::list<std::string> out;
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);
		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);
	}
	TEST(QueryResult, addResultMergeFilterSingleColumnSingleTable) {
		Entity e(STMT, Synonym{ "x" });
		std::vector<StmtInfo> a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::vector<StmtInfo> a2 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		ResultTable t1(e, a1);
		ResultTable t2(e, a2);
		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		std::list<std::string> b = { "1", "3", "5", "7" };

		std::list<std::string> out;
		q.getResult(e, out);
		EXPECT_EQ(out, b);
		q.addResult(t2);

		out.clear();
		q.getResult(e, out);
		EXPECT_EQ(out, b);

		a1 = { { 1, STMT_WHILE } , { 3, STMT_READ }, { 5, STMT_CALL }, { 7, STMT_ASSIGN} };
		a2 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);

		out.clear();
		q.getResult(e, out);
		EXPECT_EQ(out, b);

		q.addResult(t2);

		out.clear();
		q.getResult(e, out);
		EXPECT_EQ(out, b);

		a1 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		a2 = { { 8, STMT_WHILE } , { 9, STMT_READ }, { 10, STMT_CALL }, { 11, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());
		q.addResult(t1);
		EXPECT_FALSE(q.haveResult());

		a1 = { { 8, STMT_WHILE } , { 9, STMT_READ }, { 10, STMT_CALL }, { 11, STMT_ASSIGN} };
		a2 = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		t1 = ResultTable(e, a1);
		t2 = ResultTable(e, a2);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());
		q.addResult(t1);
		EXPECT_FALSE(q.haveResult());
	}

	TEST(QueryResult, addResultMergeFilterSingleColumnDoubleTable) {
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

		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		std::list<std::string> b1 = { "9", "1", "5" };
		std::list<std::string> b2 = { "2", "6", "10" };


		std::list<std::string> out;
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);

		out.clear();
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);


		e2 = { STMT, Synonym{ "y" } };
		a2 = { { 4, STMT_WHILE }, { 8, STMT_CALL } };

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		b1 = { "3", "7" };
		b2 = { "4", "8" };

		out.clear();
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);

		a2 = { { 4, STMT_WHILE }, { 8, STMT_CALL } };

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);

		out.clear();
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);

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

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		b1 = { "3", "7" };
		b2 = { "4", "8" };
		EXPECT_FALSE(q.haveResult());

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());

		e2 = { STMT, Synonym{ "y" } };

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());

		t1 = ResultTable(e2, a2);
		t2 = ResultTable(e1, a1);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());
	}
	TEST(QueryResult, addResultMergeFilterDoubleColumnDoubleTable) {
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

		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		std::list<std::string> b1 = { "9", "1", "5" };
		std::list<std::string> b2 = { "2", "6", "10" };

		std::list<std::string> out;
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);

		e2 = { {STMT, Synonym{"y"} } , { STMT,Synonym{"x"} } };
		a2 = {
			{ "2", "1", },
			{ "6", "5", },
			{ "10", "9", }
		};

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		b1 = { "9", "1", "5" };
		b2 = { "2", "6", "10" };

		out.clear();
		q.getResult(e1.first, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e1.second, out);
		EXPECT_EQ(out, b2);


		e2 = { {STMT, Synonym{"y"} } , { STMT,Synonym{"x"} } };
		a2 = {
			{ "2", "2", },
			{ "6", "2", },
			{ "10", "2", }
		};

		t1 = ResultTable(e1, a1);
		t2 = ResultTable(e2, a2);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		EXPECT_FALSE(q.haveResult());
	}

	TEST(QueryResult, addResultMergeFilterDoubleColumnTripleTable) {
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

		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		std::list<std::string> b1 = { "1", "2" };
		std::list<std::string> b2 = { "4", "5" };
		std::list<std::string> b3 = { "7", "8" };

		std::list<std::string> out;
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t2 = ResultTable(h3, a3);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

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
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		EXPECT_FALSE(q.haveResult());
	}

	TEST(QueryResult, addResultMergeJoinTableSingleColumn) {
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
		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		std::list<std::string> b1 = { "1", "2", "3" };
		std::list<std::string> b2 = { "4", "5", "6" };
		std::list<std::string> b3 = { "7", "8", "9" };

		std::list<std::string> out;
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

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
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		q = QueryResult();
		q.addResult(t2);
		q.addResult(t1);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

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
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		q = QueryResult();
		q.addResult(t2);
		q.addResult(t1);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);
	}

	TEST(QueryResult, addResultMergeJoinDoubleColumnTripleTable) {
		Entity e1 = { STMT, Synonym{"x"} };
		Entity e2 = { STMT, Synonym{"y"} };
		Entity e3 = { STMT, Synonym{"z"} };
		Entity e4 = { STMT, Synonym{"v"} };
		std::pair<Entity, Entity> h1{ e1, e2 };
		std::pair<Entity, Entity> h2{ e2, e3 };
		std::pair<Entity, Entity> h3{ e1, e2 };
		std::pair<Entity, Entity> h4{ e2, e4 };
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
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};
		std::vector<std::pair<std::string, std::string>> a4{
			{ "4", "a", },
			{ "5", "b", },
			{ "6", "c", }
		};

		ResultTable t1(h1, a1);
		ResultTable t2(h2, a2);
		ResultTable t3(h3, a3);
		ResultTable t4(h4, a4);
		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		q.addResult(t4);
		std::list<std::string> b1 = { "1", "2", "3" };
		std::list<std::string> b2 = { "4", "5", "6" };
		std::list<std::string> b3 = { "7", "8", "9" };
		std::list<std::string> b4 = { "a", "b", "c" };

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t4);
		q.addResult(t3);

		std::list<std::string> out;
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t4);
		q.addResult(t2);
		q.addResult(t3);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t4);
		q.addResult(t3);
		q.addResult(t2);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t4);
		q.addResult(t2);
		q.addResult(t3);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t4);
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		h1 = { e1, e2 };
		h2 = { e3, e2 };
		h3 = { e1, e3 };
		h4 = { e3, e4 };
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
			{ "1", "7", },
			{ "3", "9", }
		};
		a4 = {
			{ "7", "10", },
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);

		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		q.addResult(t4);
		b1 = { "1" };
		b2 = { "4" };
		b3 = { "7" };
		b4 = { "10" };

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		h1 = { e1, e2 };
		h2 = { e2, e3 };
		h3 = { e2, e3 };
		h4 = { e3, e4 };
		a1 = {
			{ "1", "1", },
			{ "1", "2", },
			{ "1", "3", },
		};
		a2 = {
			{ "1", "1", },
			{ "1", "2", },
			{ "1", "3", }
		};
		a3 = {
			{ "1", "1", },
			{ "2", "1", },
		};
		a4 = {
			{ "1", "4", },
			{ "1", "5", },
			{ "1", "6", }
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		q.addResult(t4);
		b1 = { "1" };
		b2 = { "1" };
		b3 = { "1" };
		b4 = { "4", "5", "6" };

		out.clear();
		q.getResult(e1, out);
		EXPECT_EQ(out, b1);

		out.clear();
		q.getResult(e2, out);
		EXPECT_EQ(out, b2);

		out.clear();
		q.getResult(e3, out);
		EXPECT_EQ(out, b3);

		out.clear();
		q.getResult(e4, out);
		EXPECT_EQ(out, b4);

		h1 = { e1, e2 };
		h2 = { e2, e3 };
		h3 = { e2, e3 };
		h4 = { e3, e4 };
		a1 = {
			{ "1", "1", },
			{ "1", "2", },
			{ "1", "3", },
		};
		a2 = {
			{ "1", "1", },
			{ "1", "2", },
			{ "1", "3", }
		};
		a3 = {
			{ "1", "1", },
			{ "2", "1", },
		};
		a4 = {
			{ "2", "4", },
			{ "2", "5", },
			{ "2", "6", }
		};

		t1 = ResultTable(h1, a1);
		t2 = ResultTable(h2, a2);
		t3 = ResultTable(h3, a3);
		t4 = ResultTable(h4, a4);
		q = QueryResult();
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);
		q.addResult(t4);
		EXPECT_FALSE(q.haveResult());
	}


	TEST(QueryResult, getResultTable) {
		Entity e1 = { STMT, Synonym{"x"} };
		Entity e2 = { STMT, Synonym{"y"} };
		Entity e3 = { STMT, Synonym{"z"} };
		Entity e4 = { STMT, Synonym{"v"} };
		Entity e5 = { STMT, Synonym{"a"} };
		Entity e6 = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> h1{ e1, e2 };
		std::pair<Entity, Entity> h2{ e3, e4 };
		std::pair<Entity, Entity> h3{ e5, e6 };
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
			{ "1", "4", },
			{ "2", "5", },
			{ "3", "6", },
		};

		ResultTable t1(h1, a1);
		ResultTable t2(h2, a2);
		ResultTable t3(h3, a3);
		QueryResult q;
		q.addResult(t1);
		q.addResult(t2);
		q.addResult(t3);

		std::vector<Entity> selected{ e1, e5 };
		ResultTable result;
		q.getResults(selected, result);
		std::list<std::string> b1 = { "1", "2", "3" };


		std::list<std::string> out;
		result.getEntityResult(e1, out);
		EXPECT_EQ(out, b1);
		out.clear();
		result.getEntityResult(e5, out);
		EXPECT_EQ(out, b1);

	}
}