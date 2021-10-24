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

	TEST(Utility, stmtIndexToStringList) {
		std::vector<stmt_index> a = { 1, 2, 3, 4 };
		std::list<std::string> b{ "1", "2", "3", "4" };
		EXPECT_EQ(Utility::stmtIndexToStringList(a), b);
	}

	TEST(Utility, pairToStringTable) {
		std::vector<std::pair<std::string, std::string>> p1 = { {"a", "b"},{"c", "d"} };
		std::list<std::vector<std::string>> t1 = { {"a", "b"},{"c", "d"} };
		EXPECT_EQ(Utility::pairToStringTable(p1), t1);

		std::vector<std::pair<stmt_index, std::string>> p2 = { { 1 , "b"}, { 2 , "d"} };
		std::list<std::vector<std::string>> t2 = { {"1", "b"},{"2", "d"} };
		EXPECT_EQ(Utility::pairToStringTable(p2), t2);

		std::vector<std::pair<StmtInfo, std::string>> p3 = { {{ 1, STMT_ASSIGN }, "b"},{{ 2, STMT_ASSIGN }, "d"} };
		std::list<std::vector<std::string>> t3 = { {"1", "b"},{"2", "d"} };
		EXPECT_EQ(Utility::pairToStringTable(p3), t3);

		std::vector<std::pair<StmtInfo, StmtInfo>> p4 = { {{ 1, STMT_ASSIGN }, { 3, STMT_WHILE }},
			{{ 2, STMT_ASSIGN }, { 4, STMT_WHILE }} };
		std::list<std::vector<std::string>> t4 = { {"1", "3"},{"2", "4"} };
		EXPECT_EQ(Utility::pairToStringTable(p4), t4);
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

	TEST(Utility, filterResultsPairStmtInfoAndString) {
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, "a"},{{ 2, STMT_IF }, "b"} ,
			{{ 3, STMT_READ }, "c"},{{ 4, STMT_PRINT }, "d"} ,
			{{ 5, STMT_CALL }, "e"},{{ 6, STMT_ASSIGN }, "f"} ,
			{{ 7, STMT_ASSIGN }, "g"},{{ 8, STMT_ASSIGN }, "h"} };

		std::list<std::vector<std::string>> b = { {"6", "f"}, {"7", "g"}, { "8", "h"} };
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
	}

	TEST(Utility, filterResultsPairStmtInfoAndStmtInfo) {
		std::vector<std::pair<StmtInfo, StmtInfo>> a = {
			{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };

		std::list<std::vector<std::string>> b = { {"6", "16"}, {"7", "17"}, { "8", "18"} };
		EXPECT_EQ(Utility::filterResults({ ASSIGN, IF }, a), b);

		b = { {"1", "11"} };
		EXPECT_EQ(Utility::filterResults({ WHILE, READ }, a), b);

		b = { {"2", "12"} };
		EXPECT_EQ(Utility::filterResults({ IF, IF }, a), b);

		b = { {"3", "13"} };
		EXPECT_EQ(Utility::filterResults({ READ, CALL }, a), b);

		b = { {"4", "14"} };
		EXPECT_EQ(Utility::filterResults({ PRINT, IF }, a), b);

		b = { {"5", "15"} };
		EXPECT_EQ(Utility::filterResults({ CALL, ASSIGN }, a), b);

		b = { };

		EXPECT_EQ(Utility::filterResults({ WHILE, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ WHILE, CALL }, a), b);
		EXPECT_EQ(Utility::filterResults({ WHILE, IF }, a), b);
		EXPECT_EQ(Utility::filterResults({ WHILE, PRINT }, a), b);
		EXPECT_EQ(Utility::filterResults({ WHILE, ASSIGN }, a), b);

		EXPECT_EQ(Utility::filterResults({ READ, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ READ, READ }, a), b);
		EXPECT_EQ(Utility::filterResults({ READ, IF }, a), b);
		EXPECT_EQ(Utility::filterResults({ READ, PRINT }, a), b);
		EXPECT_EQ(Utility::filterResults({ READ, ASSIGN }, a), b);

		EXPECT_EQ(Utility::filterResults({ CALL, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ CALL, READ }, a), b);
		EXPECT_EQ(Utility::filterResults({ CALL, CALL }, a), b);
		EXPECT_EQ(Utility::filterResults({ CALL, IF }, a), b);
		EXPECT_EQ(Utility::filterResults({ CALL, PRINT }, a), b);

		EXPECT_EQ(Utility::filterResults({ IF, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ IF, READ }, a), b);
		EXPECT_EQ(Utility::filterResults({ IF, CALL }, a), b);
		EXPECT_EQ(Utility::filterResults({ IF, PRINT }, a), b);
		EXPECT_EQ(Utility::filterResults({ IF, ASSIGN }, a), b);

		EXPECT_EQ(Utility::filterResults({ PRINT, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ PRINT, READ }, a), b);
		EXPECT_EQ(Utility::filterResults({ PRINT, CALL }, a), b);
		EXPECT_EQ(Utility::filterResults({ PRINT, PRINT }, a), b);
		EXPECT_EQ(Utility::filterResults({ PRINT, ASSIGN }, a), b);

		EXPECT_EQ(Utility::filterResults({ ASSIGN, WHILE }, a), b);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, READ }, a), b);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, CALL }, a), b);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, PRINT }, a), b);
		EXPECT_EQ(Utility::filterResults({ ASSIGN, ASSIGN }, a), b);
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

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);

		filters = { "2", "4" };;
		to = {
			{"2", "2", "3", "4", "5"},
			{"2", "3", "3", "4", "5"},
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"}
		};

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);

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

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);

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

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);

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

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);

		filters = { "2", "4" };;
		to = { };

		EXPECT_EQ(Utility::filterResults(from, filters, index), to);
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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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
			{"4", "4", "3", "4", "5"},
			{"4", "5", "3", "4", "5"}
		};

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

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
			{"4", "5", "3", "4", "5"},
		};

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

		index1 = 3;
		index2 = 4;
		filters = {
			{"1", {"1", "2" } },
			{"2", {"2", "3" } },
			{"3", {"3" } },
			{"4", {"1", "2" } },
			{"5", {"5" } }
		};

		to = { };

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);

		index1 = 1;
		index2 = 4;
		filters = {
			{"1", {"4", "5" } },
			{"2", {"3", "5" } },
			{"3", {"3" } },
			{"4", {"5", "4" } },
			{"5", {"5" } }
		};

		EXPECT_EQ(Utility::filterResults(to, filters, index1, index2), to);
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
		std::list<std::string> r2, r3;

		for (int i1 = 0; i1 < 10; i1++) {
			for (int i2 = 0; i2 < 10; i2++) {
				for (int i3 = 0; i3 < 10; i3++) {
					t2.push_back({ std::to_string(i1), std::to_string(i2), std::to_string(i3) });
				}
			}
			r2.push_back(std::to_string(i1));
		}

		EXPECT_EQ(Utility::mergeColumnEqual(t2), r2);

		for (int i1 = 0; i1 < 20; i1++) {
			for (int i2 = 0; i2 < 20; i2++) {
				for (int i3 = 0; i3 < 20; i3++) {
					for (int i4 = 0; i4 < 20; i4++) {
						t3.push_back({ std::to_string(i1), std::to_string(i2),
							std::to_string(i3),std::to_string(i4) });
					}
				}
			}
			r3.push_back(std::to_string(i1));
		}

		EXPECT_EQ(Utility::mergeColumnEqual(t3), r3);
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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

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

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);

		fromIndex = 0;
		to_join_index = 1;

		to = {
			{"1", "1", "5" },
			{"1", "1", "4" },
			{"1", "1", "2" },
			{"1", "1", "1" }
		};

		EXPECT_EQ(Utility::joinTable(from, fromIndex, to_join, to_join_index), to);
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