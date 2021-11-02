#include "pch.h"

#include "Utility.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Utility, constantsToStringList) {
		std::vector<constant> a{ 1, 2, 3, 4, 5 };
		std::list<std::string> b{ "1", "2", "3", "4", "5" };

		std::list<std::string> out;
		Utility::constantToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ "1", "2", "3", "4" };
		EXPECT_NE(out, c);

		std::vector<constant> d{ 1, 2 };
		std::list<std::string> e{ "1", "2" };

		out.clear();
		Utility::constantToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST(Utility, proceduresToStringList) {
		std::vector<proc_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		std::list<std::string> out;
		Utility::procedureToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(out, c);

		std::vector<proc_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		out.clear();
		Utility::procedureToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST(Utility, stmtInfoToStringList) {
		std::vector<StmtInfo> a{ {1, STMT_ASSIGN}, {2, STMT_CALL} ,
			{3, STMT_IF} ,{4, STMT_WHILE} ,{5, STMT_PRINT} };
		std::list<std::string> b{ "1", "2", "3", "4", "5" };

		std::list<std::string> out;
		Utility::stmtInfoToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ "1", "2", "3", "4" };
		EXPECT_NE(out, c);

		std::vector<StmtInfo> d{ {1, STMT_ASSIGN}, {2, STMT_CALL} };
		std::list<std::string> e{ "1", "2" };

		out.clear();
		Utility::stmtInfoToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST(Utility, variablesToStringList) {
		std::vector<var_name> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		std::list<std::string> out;
		Utility::variableToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(out, c);

		std::vector<var_name> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		out.clear();
		Utility::variableToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST(Utility, unorderedSetToStringList) {
		std::unordered_set<std::string> a{ "a", "b", "c", "d", "e" };
		std::list<std::string> b{ "a", "b", "c", "d", "e" };

		std::list<std::string> out;
		Utility::unorderedSetToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ "a", "b", "c", "d" };
		EXPECT_NE(out, c);

		std::unordered_set<std::string> d{ "a", "b" };
		std::list<std::string> e{ "a", "b" };

		out.clear();
		Utility::unorderedSetToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST(Utility, stmtIndexToStringList) {
		std::vector<stmt_index> a = { 1, 2, 3, 4 };
		std::list<std::string> b{ "1", "2", "3", "4" };
		std::list<std::string> out;
		Utility::stmtIndexToStringList(a, out);
		EXPECT_EQ(out, b);
	}

	TEST(Utility, stringListToStringPair) {
		std::list<std::string> a{ "1", "2", "3", "4" };
		std::list<std::vector<std::string>> b = { {"1", "1"},{"2", "2"},{"3", "3"},{"4", "4"} };

		std::list<std::vector<std::string>> out;
		Utility::stringListToStringTablePair(a, out);
		EXPECT_EQ(out, b);
	}


	TEST(Utility, pairToStringTable) {
		std::vector<std::pair<std::string, std::string>> p1 = { {"a", "b"},{"c", "d"} };
		std::list<std::vector<std::string>> t1 = { {"a", "b"},{"c", "d"} };

		std::list<std::vector<std::string>> out; 
		Utility::pairToStringTable(p1, out);
		EXPECT_EQ(out, t1);

		std::vector<std::pair<stmt_index, std::string>> p2 = { { 1 , "b"}, { 2 , "d"} };
		std::list<std::vector<std::string>> t2 = { {"1", "b"},{"2", "d"} };
		out.clear();
		Utility::pairToStringTable(p2, out);
		EXPECT_EQ(out, t2);

		std::vector<std::pair<StmtInfo, std::string>> p3 = { {{ 1, STMT_ASSIGN }, "b"},{{ 2, STMT_ASSIGN }, "d"} };
		std::list<std::vector<std::string>> t3 = { {"1", "b"},{"2", "d"} };

		out.clear();
		Utility::pairToStringTable(p3, out);
		EXPECT_EQ(out, t3);

		std::vector<std::pair<StmtInfo, StmtInfo>> p4 = { {{ 1, STMT_ASSIGN }, { 3, STMT_WHILE }},
			{{ 2, STMT_ASSIGN }, { 4, STMT_WHILE }} };
		std::list<std::vector<std::string>> t4 = { {"1", "3"},{"2", "4"} };

		out.clear();
		Utility::pairToStringTable(p4, out);
		EXPECT_EQ(out, t4);
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
		std::vector<Entity> e{ {WHILE, Synonym{"test"} }, { READ,Synonym{"hello"} }, { PRINT,Synonym{"test1"} }, { STMT,Synonym{"test3"} } };

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
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
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


	TEST(Utility, filterResultsToTable) {
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::list<std::vector<std::string>> b = { { "6" } , { "7" } };

		std::list<std::vector<std::string>> out;
		Utility::filterResults(ASSIGN, a, out);
		EXPECT_EQ(out, b);

		b = { { "1" } };
		out.clear();
		Utility::filterResults(WHILE, a, out);
		EXPECT_EQ(out, b);

		b = { { "2" } };
		out.clear();
		Utility::filterResults(IF, a, out);
		EXPECT_EQ(out, b);

		b = { { "3" } };
		out.clear();
		Utility::filterResults(READ, a, out);
		EXPECT_EQ(out, b);

		b = { { "4" } };
		out.clear();
		Utility::filterResults(PRINT, a, out);
		EXPECT_EQ(out, b);

		b = { { "5" } };
		out.clear();
		Utility::filterResults(CALL, a, out);
		EXPECT_EQ(out, b);
	}
	TEST(Utility, filterResultsPairStmtInfoAndString) {
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };

		std::list<std::vector<std::string>> b = { {"6", "f"}, {"7", "g"}, { "8", "h"} };

		std::list<std::vector<std::string>> out;
		Utility::filterResults(ASSIGN, a, out);
		EXPECT_EQ(out, b);
		EXPECT_EQ(out, b);

		b = { {"1", "a"} };

		out.clear();
		Utility::filterResults(WHILE, a, out);
		EXPECT_EQ(out, b);

		b = { {"2", "b"} };

		out.clear();
		Utility::filterResults(IF, a, out);
		EXPECT_EQ(out, b);

		b = { {"3", "c"} };

		out.clear();
		Utility::filterResults(READ, a, out);
		EXPECT_EQ(out, b);

		b = { {"4", "d"} };

		out.clear();
		Utility::filterResults(PRINT, a, out);
		EXPECT_EQ(out, b);

		b = { {"5", "e"} };


		out.clear();
		Utility::filterResults(CALL, a, out);
		EXPECT_EQ(out, b);
	}

	TEST(Utility, filterResultsPairStmtInfoAndStmtInfo) {
		std::vector<std::pair<StmtInfo, StmtInfo>> a = {
			{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };

		std::list<std::vector<std::string>> b = { {"6", "16"}, {"7", "17"}, { "8", "18"} };

		std::list<std::vector<std::string>> out;
		Utility::filterResults({ ASSIGN, IF }, a, out);
		EXPECT_EQ(out, b);


		b = { {"1", "11"} };

		out.clear();
		Utility::filterResults({ WHILE, READ }, a, out);
		EXPECT_EQ(out, b);


		b = { {"2", "12"} };

		out.clear();
		Utility::filterResults({ IF, IF }, a, out);
		EXPECT_EQ(out, b);

		b = { {"3", "13"} };

		out.clear();
		Utility::filterResults({ READ, CALL }, a, out);
		EXPECT_EQ(out, b);

		b = { {"4", "14"} };

		out.clear();
		Utility::filterResults({ PRINT, IF }, a, out);
		EXPECT_EQ(out, b);

		b = { {"5", "15"} };

		out.clear();
		Utility::filterResults({ CALL, ASSIGN }, a, out);
		EXPECT_EQ(out, b);

		b = { };

		std::vector<std::pair<EntityType, EntityType>> filter;
		filter.push_back({ WHILE, WHILE });
		filter.push_back({ WHILE, CALL });
		filter.push_back({ WHILE, IF });
		filter.push_back({ WHILE, PRINT });
		filter.push_back({ WHILE, ASSIGN });

		filter.push_back({ READ, WHILE });
		filter.push_back({ READ, READ });
		filter.push_back({ READ, IF });
		filter.push_back({ READ, PRINT });
		filter.push_back({ READ, ASSIGN });

		filter.push_back({ CALL, WHILE });
		filter.push_back({ CALL, READ });
		filter.push_back({ CALL, CALL });
		filter.push_back({ CALL, IF });
		filter.push_back({ CALL, PRINT });

		filter.push_back({ IF, WHILE });
		filter.push_back({ IF, READ });
		filter.push_back({ IF, CALL });
		filter.push_back({ IF, PRINT });
		filter.push_back({ IF, ASSIGN });

		filter.push_back({ PRINT, WHILE });
		filter.push_back({ PRINT, READ });
		filter.push_back({ PRINT, CALL });
		filter.push_back({ PRINT, PRINT });
		filter.push_back({ PRINT, ASSIGN });

		filter.push_back({ ASSIGN, WHILE });
		filter.push_back({ ASSIGN, READ });
		filter.push_back({ ASSIGN, CALL });
		filter.push_back({ ASSIGN, PRINT });
		filter.push_back({ ASSIGN, ASSIGN });

		for (auto f : filter) {
			out.clear();
			Utility::filterResults(f, a, out);
			EXPECT_EQ(out, b);
		}
	}

	TEST(Utility, filterResultsSingleColumn) {
		std::list<std::vector<std::string>> from = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		int index = 0;
		std::unordered_set<std::string> filters = { "1", "3", "5" };;
		std::list<std::vector<std::string>> to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};


		std::list<std::vector<std::string>> out; 
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);


		filters = { "2", "4" };;
		to = {
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		index = 2;
		filters = { "1", "3", "5" };;
		to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		filters = { "3" };;
		to = {
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		index = 4;
		filters = { "1", "5" };;
		to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		filters = { "2", "4" };;
		to = { };

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);
	}

	TEST(Utility, filterResultsDoubleColumn) {
		std::list<std::vector<std::string>> from = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		int index1 = 0;
		int index2 = 1;
		std::unordered_map<std::string, std::unordered_set<std::string>> filters = {
			{"1", {"1", "2" } },
			{"2", {"2", "3" } },
			{"3", {"3" } },
			{"4", {"4", "5" } },
			{"5", {"5" } }
		};

		std::list<std::vector<std::string>> to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		std::list<std::vector<std::string>> out;
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);


		filters = {
			{"1", {"1", "2" } },
			{"3", {"3" } },
			{"5", {"5" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		filters = {
			{"3", {"3" } },
			{"4", {"4" } },
			{"5", {"5" } }
		};

		to = {
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 2;

		filters = {
			{"1", {"1", "2" } },
			{"2", {"2", "3" } },
			{"3", {"3" } },
			{"4", {"4", "5" } },
			{"5", {"5" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 3;
		filters = {
			{"1", {"1", "2" } },
			{"2", {"2", "3" } },
			{"3", {"4" } },
			{"4", {"4", "5" } },
			{"5", {"5" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 4;
		filters = {
			{"1", {"1", "2" } },
			{"3", {"5" } },
			{"4", {"5" } },
			{"5", {"5" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"2", "3", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 0;
		index2 = 3;
		filters = {
			{"1", {"1", "4" } },
			{"2", {"2", "3" } },
			{"3", {"4" } },
			{"4", {"4", "5" } },
			{"5", {"4" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"1", "2", "3", "4", "5"},
			{"3", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"},
			{"5", "5", "3", "4", "5"}
		};


		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 0;
		index2 = 4;
		filters = {
			{"1", {"1", "2" } },
			{"2", {"1", "3" } },
			{"3", {"1" } },
			{"4", {"1", "5" } },
			{"5", {"1" } }
		};

		to = {
			{"1", "1", "1", "1", "1"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 3;
		index2 = 4;
		filters = {
			{"1", {"1", "2" } },
			{"2", {"2", "3" } },
			{"3", {"3" } },
			{"4", {"1", "2" } },
			{"5", {"5" } }
		};

		to = { {"1", "1", "1", "1", "1"} };

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 4;
		filters = {
			{"1", {"3", "4" } },
			{"2", {"3", "4" } },
			{"3", {"3" } },
			{"4", {"3", "4" } },
			{"5", {"2" } }
		};

		to = { };

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);
	}
	
	TEST(Utility, mergeColumnEqual) {
		std::list<std::vector<std::string>> t1 = {
			{"1", "1"},{"1", "2"},{"1", "3"},{"1", "4"},{"1", "5"},
			{"2", "1"},{"2", "2"},{"2", "3"},{"2", "4"},{"2", "5"},
			{"3", "1"},{"3", "2"},{"3", "3"},{"3", "4"},{"3", "5"},
			{"4", "1"},{"4", "2"},{"4", "3"},{"4", "5"},
			{"5", "1"},{"5", "2"},{"5", "3"},{"5", "4"},{"5", "5"}
		};

		std::list<std::string> r1 = { "1", "2", "3", "5" };

		std::list<std::vector<std::string>> t2, t3;
		std::list<std::vector<std::string>> r2, r3;

		for (int i1 = 0; i1 < 10; i1++) {
			for (int i2 = 0; i2 < 10; i2++) {
				for (int i3 = 0; i3 < 10; i3++) {
					t2.push_back({ std::to_string(i1), std::to_string(i2), std::to_string(i3) });
				}
			}
			r2.push_back({ std::to_string(i1) });
		}

		Utility::mergeColumnEqual(t2);
		EXPECT_EQ(t2, r2);

		for (int i1 = 0; i1 < 20; i1++) {
			for (int i2 = 0; i2 < 20; i2++) {
				for (int i3 = 0; i3 < 20; i3++) {
					for (int i4 = 0; i4 < 20; i4++) {
						t3.push_back({ std::to_string(i1), std::to_string(i2),
							std::to_string(i3),std::to_string(i4) });
					}
				}
			}
			r3.push_back({ std::to_string(i1) });
		}

		Utility::mergeColumnEqual(t3);
		EXPECT_EQ(t3, r3);
	}

	TEST(Utility, joinTableSingleColumn) {
		std::list<std::vector<std::string>> from = {
			{"1", "1"},
			{"1", "2"},
			{"1", "3"},
			{"2", "1"},
			{"2", "2"},
			{"2", "3"},
			{"3", "1"},
			{"3", "2"},
			{"3", "3"},
		};

		std::unordered_multimap<std::string, std::vector<std::string>> to_join = {
			{"1", { "1", "1" } },
			{"2", { "2", "2" } },
			{"3", { "3", "3" } },
			{"4", { "4", "4" } },
			{"5", { "5", "5" } }
		};

		int fromIndex = 0;
		int to_join_index = 0;

		std::list<std::vector<std::string>> to = {
			{"1", "1", "1"},
			{"1", "2", "1"},
			{"1", "3", "1"},
			{"2", "1", "2"},
			{"2", "2", "2"},
			{"2", "3", "2"},
			{"3", "1", "3"},
			{"3", "2", "3"},
			{"3", "3", "3"},
		};

		std::list<std::vector<std::string>> out;
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 1;
		to_join_index = 0;

		to = {
			{"1", "1", "1"},
			{"1", "2", "2"},
			{"1", "3", "3"},
			{"2", "1", "1"},
			{"2", "2", "2"},
			{"2", "3", "3"},
			{"3", "1", "1"},
			{"3", "2", "2"},
			{"3", "3", "3"},
		};


		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{"1"},
			{"2"},
			{"3"}
		};

		to_join = {
			{"1", { "1", "1" } },
			{"1", { "1", "2" } },
			{"2", { "2", "3" } },
			{"1", { "1", "4" } },
			{"1", { "1", "5" } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{"1", "1"},
			{"1", "2"},
			{"1", "4"},
			{"1", "5"},
			{"2", "3"}
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{"1"},
			{"2"},
			{"3"}
		};

		to_join = {
			{"1", { "1", "1", "5" } },
			{"1", { "1", "2", "4" } },
			{"2", { "2", "3", "3" } },
			{"1", { "1", "4", "2" } },
			{"1", { "1", "5", "1" } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{"1", "1", "5" },
			{"1", "2", "4" },
			{"1", "4", "2" },
			{"1", "5", "1" },
			{"2", "3", "3" }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{"1", "1"},
			{"1", "2"},
			{"1", "3"},
			{"2", "1"},
			{"2", "2"},
			{"2", "3"},
			{"3", "1"},
			{"3", "2"},
			{"3", "3"}
		};

		to_join = {
			{"1", { "1", "1", "5" } },
			{"1", { "1", "2", "4" } },
			{"2", { "2", "3", "3" } },
			{"1", { "1", "4", "2" } },
			{"1", { "1", "5", "1" } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{"1", "1", "1", "5" },
			{"1", "1", "2", "4" },
			{"1", "1", "4", "2" },
			{"1", "1", "5", "1" },
			{"1", "2", "1", "5" },
			{"1", "2", "2", "4" },
			{"1", "2", "4", "2" },
			{"1", "2", "5", "1" },
			{"1", "3", "1", "5" },
			{"1", "3", "2", "4" },
			{"1", "3", "4", "2" },
			{"1", "3", "5", "1" },
			{"2", "1", "3", "3" },
			{"2", "2", "3", "3" },
			{"2", "3", "3", "3" }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{"1"},
			{"2"},
			{"3"},
			{"4"},
			{"5"},
			{"6"},
			{"7"},
			{"8"},
			{"9"}
		};

		to_join = {
			{"1", { "1", "1", "5" } },
			{"1", { "1", "2", "4" } },
			{"1", { "1", "4", "2" } },
			{"1", { "1", "5", "1" } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{"1", "1", "5" },
			{"1", "2", "4" },
			{"1", "4", "2" },
			{"1", "5", "1" }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 0;
		to_join_index = 1;

		to = {
			{"1", "1", "5" },
			{"1", "1", "4" },
			{"1", "1", "2" },
			{"1", "1", "1" }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);
	}

	TEST(Utility, joinTable) {
		std::list<std::vector<std::string>> a = { {"1"}, {"2"}, {"3"} };
		std::list<std::vector<std::string>> b = { {"1"} };
		std::list<std::vector<std::string>> to = { {"1","1"}, {"2","1"}, {"3","1"} };

		std::list<std::vector<std::string>> out;
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);


		a = { {"1"} };
		b = { {"1"}, {"2"}, {"3"} };
		to = { {"1","1"}, {"1","2"}, {"1","3"} };

		out.clear();
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);


		a = { {"1"}, {"2"}, {"3"} };
		b = { {"1"}, {"2"}, {"3"} };
		to = { {"1","1"}, {"1","2"}, {"1","3"},
				{"2","1"}, {"2","2"}, {"2","3"},
				{"3","1"}, {"3","2"}, {"3","3"} };

		out.clear();
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);
	}

	TEST(Utility, getEntitiesExclude) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::vector<Entity> a = { e1, e2, e3, e4, e5 };
		std::vector<Entity> b = { e1 };
		std::vector<Entity> to = { e2, e3, e4, e5 };
		EXPECT_EQ(Utility::getEntitiesExclude(a, b), to);

		b = { e2, e4 };
		to = { e1, e3, e5 };
		EXPECT_EQ(Utility::getEntitiesExclude(a, b), to);

		b = { e1, e3, e5 };
		to = { e2, e4 };
		EXPECT_EQ(Utility::getEntitiesExclude(a, b), to);

		b = { e1, e3, e4, e5 };
		to = { e2 };
		EXPECT_EQ(Utility::getEntitiesExclude(a, b), to);

		b = { e1, e2, e3, e4, e5 };
		to = { };
		EXPECT_EQ(Utility::getEntitiesExclude(a, b), to);
	}


	TEST(Utility, getEntitiesWithoutDuplicate) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::vector<Entity> a = { e1, e2, e1, e3, e2, e3, e4, e5, e1, e2, e3, e4, e5 };
		std::vector<Entity> b = { e1, e2, e3, e4, e5 };
		EXPECT_EQ(Utility::getEntitiesWithoutDuplicate(a), b);
	}

	TEST(Utility, isSecondaryAttribute) {
		Entity e = { CALL,Synonym{"e1"}, PROC_NAME };
		EXPECT_TRUE(Utility::isSecondaryAttribute(e));
		e = { READ,Synonym{"e1"}, VAR_NAME };
		EXPECT_TRUE(Utility::isSecondaryAttribute(e));
		e = { PRINT,Synonym{"e1"}, VAR_NAME };
		EXPECT_TRUE(Utility::isSecondaryAttribute(e));

		e = { STMT,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { READ,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { PRINT,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { CALL,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { WHILE,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { IF,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { ASSIGN,Synonym{"e1"}, STMT_INDEX };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { CONSTANT,Synonym{"e1"}, VALUE };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));
		e = { PROCEDURE,Synonym{"e1"}, PROC_NAME };
		EXPECT_FALSE(Utility::isSecondaryAttribute(e));

	}

	TEST(Utility, getColumnsNoDuplicate) {
		std::list<std::vector<std::string>> a = { 
			{"1","1","1"}, 
			{"1","2","1"}, 
			{"1","3","1"} };

		std::vector<int> b = { 1, 2 };
		std::list<std::vector<std::string>> to = { {"1","1"}, {"2","1"}, {"3","1"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

		b = { 0, 2 };
		to = { {"1","1"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

		b = { 0, 1 };
		to = { {"1","1"}, {"1","2"}, {"1","3"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

		b = { 0 };
		to = { {"1"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

		b = { 1 };
		to = { {"1"}, {"2"}, {"3"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

		b = { 2 };
		to = { {"1"} };
		EXPECT_EQ(Utility::getColumnsNoDuplicate(a, b), to);

	}
	TEST(Utility, queryTokenTypeToEntityType) {
		EntityType entType = STMT;
		QueryToken::QueryTokenType temp = QueryToken::QueryTokenType::STMT;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = PROCEDURE;
		temp = QueryToken::QueryTokenType::PROCEDURE;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = READ;
		temp = QueryToken::QueryTokenType::READ;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = PRINT;
		temp = QueryToken::QueryTokenType::PRINT;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = CALL;
		temp = QueryToken::QueryTokenType::CALL;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = IF;
		temp = QueryToken::QueryTokenType::IF;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = WHILE;
		temp = QueryToken::QueryTokenType::WHILE;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = ASSIGN;
		temp = QueryToken::QueryTokenType::ASSIGN;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = VARIABLE;
		temp = QueryToken::QueryTokenType::VARIABLE;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);

		entType = CONSTANT;
		EXPECT_NE(Utility::queryTokenTypeToEntityType(temp), entType);
		temp = QueryToken::QueryTokenType::CONSTANT;
		EXPECT_EQ(Utility::queryTokenTypeToEntityType(temp), entType);
	}
}