#include "pch.h"

#include "PQL/Helper/Utility.h"
#include "Common/Common.h"

namespace UnitTesting {
	class UtilityTest : public testing::Test {
	protected:
		UtilityTest() {
		}

		std::string s1 = "a";
		std::string s2 = "b";
		std::string s3 = "c";
		std::string s4 = "d";
		std::string s5 = "e";
		std::string s6 = "f";
		std::string s7 = "g";
		std::string s8 = "h";

		std::unordered_map<value, std::string> hash_out;

		value h1 = Utility::hashString(s1, hash_out);
		value h2 = Utility::hashString(s2, hash_out);
		value h3 = Utility::hashString(s3, hash_out);
		value h4 = Utility::hashString(s4, hash_out);
		value h5 = Utility::hashString(s5, hash_out);
		value h6 = Utility::hashString(s6, hash_out);
		value h7 = Utility::hashString(s7, hash_out);
		value h8 = Utility::hashString(s8, hash_out);
	};


	TEST_F(UtilityTest, unorderedSetToStringList) {
		std::unordered_set<std::string> a{ s1, s2, s3, s4, s5 };
		std::list<std::string> b{ s1, s2, s3, s4, s5 };

		std::list<std::string> out;
		Utility::unorderedSetToStringList(a, out);
		EXPECT_EQ(out, b);

		std::list<std::string> c{ s1, s2, s3, s4 };
		EXPECT_NE(out, c);

		std::unordered_set<std::string> d{ s1, s2 };
		std::list<std::string> e{ s1, s2 };

		out.clear();
		Utility::unorderedSetToStringList(d, out);
		EXPECT_EQ(out, e);
	}

	TEST_F(UtilityTest, duplicateColumn) {
		std::vector<std::string> a{ s1, s2, s3, s4 };
		std::vector<std::pair<std::string, std::string>> b = { {s1, s1},{s2, s2},{s3, s3},{s4, s4} };

		EXPECT_EQ(Utility::duplicateColumn(a), b);

		std::vector<value> c{ 1, 2, 3, 4 };
		std::vector<std::pair<value, value>> d = { {1, 1},{2, 2},{3, 3},{4, 4} };

		EXPECT_EQ(Utility::duplicateColumn(c), d);

		std::vector<StmtInfo> e = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ }, { 4, STMT_PRINT} };
		std::vector<std::pair<StmtInfo, StmtInfo>> f = { { { 1, STMT_WHILE }, { 1, STMT_WHILE } } , { { 2, STMT_IF}, { 2, STMT_IF} },
			{ { 3, STMT_READ }, { 3, STMT_READ } },{ { 4, STMT_PRINT}, { 4, STMT_PRINT} } };

		EXPECT_EQ(Utility::duplicateColumn(e), f);
	}

	TEST_F(UtilityTest, pairToTable) {
		std::vector<std::pair<std::string, std::string>> p1 = { {s1, s2},{s3, s4} };
		std::list<std::vector<value>> t1 = { {h1, h2},{h3, h4} };

		std::list<std::vector<value>> out;
		Utility::pairToTable(p1, out, hash_out);
		EXPECT_EQ(out, t1);

		std::vector<std::pair<value, std::string>> p2 = { { 1 , s2}, { 2 , s4} };
		std::list<std::vector<value>> t2 = { {1, h2},{2, h4} };
		out.clear();
		Utility::pairToTable(p2, out, hash_out);
		EXPECT_EQ(out, t2);

		std::vector<std::pair<StmtInfo, std::string>> p3 = { {{ 1, STMT_ASSIGN }, s2},{{ 2, STMT_ASSIGN }, s4} };
		std::list<std::vector<value>> t3 = { {1, h2},{2, h4} };

		out.clear();
		Utility::pairToTable(p3, out, hash_out);
		EXPECT_EQ(out, t3);

		std::vector<std::pair<StmtInfo, StmtInfo>> p4 = { {{ 1, STMT_ASSIGN }, { 3, STMT_WHILE }},
			{{ 2, STMT_ASSIGN }, { 4, STMT_WHILE }} };
		std::list<std::vector<value>> t4 = { {1, 3},{2, 4} };

		out.clear();
		Utility::pairToTable(p4, out);
		EXPECT_EQ(out, t4);
	}
	TEST_F(UtilityTest, getIndexEntity) {
		std::vector<Entity> e{ {WHILE, Synonym{"test"} }, { READ,Synonym{"hello"} }, { PRINT,Synonym{"test1"} }, { STMT,Synonym{"test3"} } };

		EXPECT_EQ(Utility::getIndex(e, e[0]), 0);
		EXPECT_EQ(Utility::getIndex(e, e[1]), 1);
		EXPECT_EQ(Utility::getIndex(e, e[2]), 2);
		EXPECT_EQ(Utility::getIndex(e, e[3]), 3);
	}

	TEST_F(UtilityTest, filterResult) {
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

	TEST_F(UtilityTest, filterResultsToTable) {
		std::vector<StmtInfo> a = { { 1, STMT_WHILE }, { 2, STMT_IF} , { 3, STMT_READ },
			{ 4, STMT_PRINT}, { 5, STMT_CALL }, { 6, STMT_ASSIGN}, { 7, STMT_ASSIGN} };
		std::list<std::vector<value>> b = { { 6 } , { 7 } };

		std::list<std::vector<value>> out;
		Utility::filterResults(ASSIGN, a, out);
		EXPECT_EQ(out, b);

		b = { { 1 } };
		out.clear();
		Utility::filterResults(WHILE, a, out);
		EXPECT_EQ(out, b);

		b = { { 2 } };
		out.clear();
		Utility::filterResults(IF, a, out);
		EXPECT_EQ(out, b);

		b = { { 3 } };
		out.clear();
		Utility::filterResults(READ, a, out);
		EXPECT_EQ(out, b);

		b = { { 4 } };
		out.clear();
		Utility::filterResults(PRINT, a, out);
		EXPECT_EQ(out, b);

		b = { { 5 } };
		out.clear();
		Utility::filterResults(CALL, a, out);
		EXPECT_EQ(out, b);
	}
	TEST_F(UtilityTest, filterResultsPairStmtInfoAndString) {
		std::vector<std::pair<StmtInfo, std::string>> a = {
			{{ 1, STMT_WHILE }, s1},{{ 2, STMT_IF }, s2} ,
			{{ 3, STMT_READ }, s3},{{ 4, STMT_PRINT }, s4} ,
			{{ 5, STMT_CALL }, s5},{{ 6, STMT_ASSIGN }, s6} ,
			{{ 7, STMT_ASSIGN }, s7},{{ 8, STMT_ASSIGN }, s8} };

		std::list<std::vector<value>> b = { {6, h6}, {7, h7}, { 8, h8} };

		std::list<std::vector<value>> out;
		Utility::filterResults(ASSIGN, a, out, hash_out);
		EXPECT_EQ(out, b);
		EXPECT_EQ(out, b);

		b = { {1, h1} };

		out.clear();
		Utility::filterResults(WHILE, a, out, hash_out);
		EXPECT_EQ(out, b);

		b = { {2, h2} };

		out.clear();
		Utility::filterResults(IF, a, out, hash_out);
		EXPECT_EQ(out, b);

		b = { {3, h3} };

		out.clear();
		Utility::filterResults(READ, a, out, hash_out);
		EXPECT_EQ(out, b);

		b = { {4, h4} };

		out.clear();
		Utility::filterResults(PRINT, a, out, hash_out);
		EXPECT_EQ(out, b);

		b = { {5, h5} };

		out.clear();
		Utility::filterResults(CALL, a, out, hash_out);
		EXPECT_EQ(out, b);
	}

	TEST_F(UtilityTest, filterResultsPairStmtInfoAndStmtInfo) {
		std::vector<std::pair<StmtInfo, StmtInfo>> a = {
			{{ 1, STMT_WHILE }, { 11, STMT_READ }},{{ 2, STMT_IF }, { 12, STMT_IF }} ,
			{{ 3, STMT_READ }, { 13, STMT_CALL }},{{ 4, STMT_PRINT }, { 14, STMT_IF }} ,
			{{ 5, STMT_CALL }, { 15, STMT_ASSIGN }},{{ 6, STMT_ASSIGN }, { 16, STMT_IF }} ,
			{{ 7, STMT_ASSIGN }, { 17, STMT_IF }},{{ 8, STMT_ASSIGN }, { 18, STMT_IF }} };

		std::list<std::vector<value>> b = { {6, 16}, {7, 17}, { 8, 18} };

		std::list<std::vector<value>> out;
		Utility::filterResults({ ASSIGN, IF }, a, out);
		EXPECT_EQ(out, b);

		b = { {1, 11} };

		out.clear();
		Utility::filterResults({ WHILE, READ }, a, out);
		EXPECT_EQ(out, b);

		b = { {2, 12} };

		out.clear();
		Utility::filterResults({ IF, IF }, a, out);
		EXPECT_EQ(out, b);

		b = { {3, 13} };

		out.clear();
		Utility::filterResults({ READ, CALL }, a, out);
		EXPECT_EQ(out, b);

		b = { {4, 14} };

		out.clear();
		Utility::filterResults({ PRINT, IF }, a, out);
		EXPECT_EQ(out, b);

		b = { {5, 15} };

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

	TEST_F(UtilityTest, filterResultsSingleColumn) {
		std::list<std::vector<value>> from = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		int index = 0;
		std::unordered_set<value> filters = { 1, 3, 5 };;
		std::list<std::vector<value>> to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		std::list<std::vector<value>> out;
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		filters = { 2, 4 };;
		to = {
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		index = 2;
		filters = { 1, 3, 5 };;
		to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		filters = { 3 };;
		to = {
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		index = 4;
		filters = { 1, 5 };;
		to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);

		filters = { 2, 4 };;
		to = { };

		out.clear();
		Utility::filterResults(from, filters, index, out);
		EXPECT_EQ(out, to);
	}

	TEST_F(UtilityTest, filterResultsDoubleColumn) {
		std::list<std::vector<value>> from = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		int index1 = 0;
		int index2 = 1;
		std::unordered_map<value, std::unordered_set<value>> filters = {
			{1, {1, 2 } },
			{2, {2, 3 } },
			{3, {3 } },
			{4, {4, 5 } },
			{5, {5 } }
		};

		std::list<std::vector<value>> to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		std::list<std::vector<value>> out;
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		filters = {
			{1, {1, 2 } },
			{3, {3 } },
			{5, {5 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		filters = {
			{3, {3 } },
			{4, {4 } },
			{5, {5 } }
		};

		to = {
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 2;

		filters = {
			{1, {1, 2 } },
			{2, {2, 3 } },
			{3, {3 } },
			{4, {4, 5 } },
			{5, {5 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{2, 2, 3, 4, 5},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 3;
		filters = {
			{1, {1, 2 } },
			{2, {2, 3 } },
			{3, {4 } },
			{4, {4, 5 } },
			{5, {5 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 4;
		filters = {
			{1, {1, 2 } },
			{3, {5 } },
			{4, {5 } },
			{5, {5 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{2, 3, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 0;
		index2 = 3;
		filters = {
			{1, {1, 4 } },
			{2, {2, 3 } },
			{3, {4 } },
			{4, {4, 5 } },
			{5, {4 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{1, 2, 3, 4, 5},
			{3, 3, 3, 4, 5},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5},
			{5, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 0;
		index2 = 4;
		filters = {
			{1, {1, 2 } },
			{2, {1, 3 } },
			{3, {1 } },
			{4, {1, 5 } },
			{5, {1 } }
		};

		to = {
			{1, 1, 1, 1, 1},
			{4, 4, 3, 4, 5},
			{4, 5, 3, 4, 5}
		};

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 3;
		index2 = 4;
		filters = {
			{1, {1, 2 } },
			{2, {2, 3 } },
			{3, {3 } },
			{4, {1, 2 } },
			{5, {5 } }
		};

		to = { {1, 1, 1, 1, 1} };

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);

		index1 = 1;
		index2 = 4;
		filters = {
			{1, {3, 4 } },
			{2, {3, 4 } },
			{3, {3 } },
			{4, {3, 4 } },
			{5, {2 } }
		};

		to = { };

		out.clear();
		Utility::filterResults(from, filters, index1, index2, out);
		EXPECT_EQ(out, to);
	}

	TEST_F(UtilityTest, mergeColumnEqual) {
		std::list<std::vector<value>> t1 = {
			{1, 1},{1, 2},{1, 3},{1, 4},{1, 5},
			{2, 1},{2, 2},{2, 3},{2, 4},{2, 5},
			{3, 1},{3, 2},{3, 3},{3, 4},{3, 5},
			{4, 1},{4, 2},{4, 3},{4, 5},
			{5, 1},{5, 2},{5, 3},{5, 4},{5, 5}
		};

		std::list<value> r1 = { 1, 2, 3, 5 };

		std::list<std::vector<value>> t2, t3;
		std::list<std::vector<value>> r2, r3;

		for (value i1 = 0; i1 < 10; i1++) {
			for (value i2 = 0; i2 < 10; i2++) {
				for (value i3 = 0; i3 < 10; i3++) {
					t2.push_back({ i1, i2, i3 });
				}
			}
			r2.push_back({ i1 });
		}

		Utility::mergeColumnEqual(t2);
		EXPECT_EQ(t2, r2);

		for (value i1 = 0; i1 < 20; i1++) {
			for (value i2 = 0; i2 < 20; i2++) {
				for (value i3 = 0; i3 < 20; i3++) {
					for (value i4 = 0; i4 < 20; i4++) {
						t3.push_back({ i1, i2, i3, i4 });
					}
				}
			}
			r3.push_back({ i1 });
		}

		Utility::mergeColumnEqual(t3);
		EXPECT_EQ(t3, r3);
	}



	TEST_F(UtilityTest, filterBothResults) {
		std::list<std::vector<value>> from1 = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3},
		};

		std::list<std::vector<value>> from2 = {
			{1, 3},
			{1, 4},
			{1, 5},
		};

		int fromIndex = 0;
		int to_join_index = 0;

		std::list<std::vector<value>> to1 = {
			{1, 1},
			{1, 2},
			{1, 3},
		};

		std::list<std::vector<value>> to2 = {
			{1, 3},
			{1, 4},
			{1, 5},
		};

		Utility::filterBothResults(from1, fromIndex, from2, to_join_index);
		EXPECT_EQ(from1, to1);
		EXPECT_EQ(from2, to2);


		from1 = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3},
		};

		from2 = {
			{1, 3},
			{1, 4},
			{1, 5},
		};

		fromIndex = 0;
		to_join_index = 1;

		to1 = {
			{3, 1},
			{3, 2},
			{3, 3}
		};

		to2 = {
			{1, 3}
		};

		Utility::filterBothResults(from1, fromIndex, from2, to_join_index);
		EXPECT_EQ(from1, to1);
		EXPECT_EQ(from2, to2);
	}


	TEST_F(UtilityTest, joinTableSingleColumn) {
		std::list<std::vector<value>> from = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3},
		};

		std::unordered_multimap<value, std::vector<value>> to_join = {
			{1, { 1, 1 } },
			{2, { 2, 2 } },
			{3, { 3, 3 } },
			{4, { 4, 4 } },
			{5, { 5, 5 } }
		};

		int fromIndex = 0;
		int to_join_index = 0;

		std::list<std::vector<value>> to = {
			{1, 1, 1},
			{1, 2, 1},
			{1, 3, 1},
			{2, 1, 2},
			{2, 2, 2},
			{2, 3, 2},
			{3, 1, 3},
			{3, 2, 3},
			{3, 3, 3},
		};

		std::list<std::vector<value>> out;
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 1;
		to_join_index = 0;

		to = {
			{1, 1, 1},
			{1, 2, 2},
			{1, 3, 3},
			{2, 1, 1},
			{2, 2, 2},
			{2, 3, 3},
			{3, 1, 1},
			{3, 2, 2},
			{3, 3, 3},
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1},
			{2},
			{3}
		};

		to_join = {
			{1, { 1, 1 } },
			{1, { 1, 2 } },
			{2, { 2, 3 } },
			{1, { 1, 4 } },
			{1, { 1, 5 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 1},
			{1, 2},
			{1, 4},
			{1, 5},
			{2, 3}
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1},
			{2},
			{3}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{2, { 2, 3, 3 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 1, 5 },
			{1, 2, 4 },
			{1, 4, 2 },
			{1, 5, 1 },
			{2, 3, 3 }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{2, { 2, 3, 3 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 1, 1, 5 },
			{1, 1, 2, 4 },
			{1, 1, 4, 2 },
			{1, 1, 5, 1 },
			{1, 2, 1, 5 },
			{1, 2, 2, 4 },
			{1, 2, 4, 2 },
			{1, 2, 5, 1 },
			{1, 3, 1, 5 },
			{1, 3, 2, 4 },
			{1, 3, 4, 2 },
			{1, 3, 5, 1 },
			{2, 1, 3, 3 },
			{2, 2, 3, 3 },
			{2, 3, 3, 3 }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = { {1}, {2}, {3},
			{4}, {5}, {6},
			{7}, {8}, {9}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 1, 5 },
			{1, 2, 4 },
			{1, 4, 2 },
			{1, 5, 1 }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 0;
		to_join_index = 1;

		to = {
			{1, 1, 5 },
			{1, 1, 4 },
			{1, 1, 2 },
			{1, 1, 1 }
		};

		out.clear();
		Utility::joinTable(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);
	}


	TEST_F(UtilityTest, joinTableExcludeJoinColumn) {
		std::list<std::vector<value>> from = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3},
		};

		std::unordered_multimap<value, std::vector<value>> to_join = {
			{1, { 1, 1 } },
			{2, { 2, 2 } },
			{3, { 3, 3 } },
			{4, { 4, 4 } },
			{5, { 5, 5 } }
		};

		int fromIndex = 0;
		int to_join_index = 0;

		std::list<std::vector<value>> to = {
			{1, 1},
			{2, 1},
			{3, 1},
			{1, 2},
			{2, 2},
			{3, 2},
			{1, 3},
			{2, 3},
			{3, 3},
		};

		std::list<std::vector<value>> out;
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 1;
		to_join_index = 0;

		to = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3},
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1},
			{2},
			{3}
		};

		to_join = {
			{1, { 1, 1 } },
			{1, { 1, 2 } },
			{2, { 2, 3 } },
			{1, { 1, 4 } },
			{1, { 1, 5 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1},
			{2},
			{4},
			{5},
			{3}
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1},
			{2},
			{3}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{2, { 2, 3, 3 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 5 },
			{2, 4 },
			{4, 2 },
			{5, 1 },
			{3, 3 }
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
			{1, 1},
			{1, 2},
			{1, 3},
			{2, 1},
			{2, 2},
			{2, 3},
			{3, 1},
			{3, 2},
			{3, 3}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{2, { 2, 3, 3 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 1, 5 },
			{1, 2, 4 },
			{1, 4, 2 },
			{1, 5, 1 },
			{2, 1, 5 },
			{2, 2, 4 },
			{2, 4, 2 },
			{2, 5, 1 },
			{3, 1, 5 },
			{3, 2, 4 },
			{3, 4, 2 },
			{3, 5, 1 },
			{1, 3, 3 },
			{2, 3, 3 },
			{3, 3, 3 }
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = { {1}, {2}, {3},
			{4}, {5}, {6},
			{7}, {8}, {9}
		};

		to_join = {
			{1, { 1, 1, 5 } },
			{1, { 1, 2, 4 } },
			{1, { 1, 4, 2 } },
			{1, { 1, 5, 1 } }
		};

		fromIndex = 0;
		to_join_index = 0;

		to = {
			{1, 5 },
			{2, 4 },
			{4, 2 },
			{5, 1 }
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		fromIndex = 0;
		to_join_index = 1;

		to = {
			{1, 5 },
			{1, 4 },
			{1, 2 },
			{1, 1 }
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);

		from = {
		{1, 1},
		{1, 2},
		{1, 3}
		};

		to_join = {
			{1, { 1, 1 } },
			{2, { 1, 2 } },
			{3, { 1, 3 } }
		};

		fromIndex = 1;
		to_join_index = 1;

		to = {
			{1, 1}
		};

		out.clear();
		Utility::joinTableExcludeJoinColumn(from, fromIndex, to_join, to_join_index, out);
		EXPECT_EQ(out, to);
	}

	TEST_F(UtilityTest, joinTable) {
		std::list<std::vector<value>> a = { {1}, {2}, {3} };
		std::list<std::vector<value>> b = { {1} };
		std::list<std::vector<value>> to = { {1,1}, {2,1}, {3,1} };

		std::list<std::vector<value>> out;
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);

		a = { {1} };
		b = { {1}, {2}, {3} };
		to = { {1,1}, {1,2}, {1,3} };

		out.clear();
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);

		a = { {1}, {2}, {3} };
		b = { {1}, {2}, {3} };
		to = { {1,1}, {1,2}, {1,3},
				{2,1}, {2,2}, {2,3},
				{3,1}, {3,2}, {3,3} };

		out.clear();
		Utility::joinTable(a, b, out);
		EXPECT_EQ(out, to);
	}

	TEST_F(UtilityTest, getEntitiesExclude) {
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

	TEST_F(UtilityTest, getEntitiesInclude) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::vector<Entity> a = { e1, e2, e3, e4, e5 };
		std::vector<Entity> b = { e1 };
		std::vector<Entity> to = { e1 };
		EXPECT_EQ(Utility::getEntitiesInclude(a, b), to);

		b = { e2, e4 };
		to = b;
		EXPECT_EQ(Utility::getEntitiesInclude(a, b), to);

		a = { e1, e2, e3 };
		b = { e1, e2, e3, e4, e5 };
		to = a;
		EXPECT_EQ(Utility::getEntitiesInclude(a, b), to);

	}

	TEST_F(UtilityTest, getSortedEntityName) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };

		std::vector<Entity> a = { e1, e2 };
		std::pair<Entity, Entity> b = { e2, e1 };

		EXPECT_EQ(Utility::getSortedEntityName(a), Utility::getSortedEntityName(b));

	}

	TEST_F(UtilityTest, getEntityListFromPair) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::list<std::pair<Entity, Entity>> p = { {e1,e2}, {e3, e4}, {e5, e1} };

		std::vector<Entity> a = { e1, e2, e3, e4, e5 };

		EXPECT_EQ(Utility::getEntityListFromPair(p), a);
	}

	TEST_F(UtilityTest, getEntityNameWithLeastFrequency) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::list<std::pair<Entity, Entity>> p = { {e1,e2}, {e1, e3}, {e3, e5}, {e2, e3},{e1,e2} };


		EXPECT_EQ(Utility::getEntityNameWithLeastFrequency(p), e5);
		EXPECT_EQ(Utility::getEntityNameWithLeastFrequency(p, { e1, e2, e3 }), e3);
	}

	TEST_F(UtilityTest, splitEntityPairs) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::list<std::pair<Entity, Entity>> p = { {e1,e2}, {e1, e3}, {e3, e5}, {e2, e3},{e1,e2} };

		std::list<std::pair<Entity, Entity>> out = Utility::splitEntityPairs(p, e1);

		std::list<std::pair<Entity, Entity>> a = { {e3, e5}, {e2, e3} };
		std::list<std::pair<Entity, Entity>> b = { {e1,e2}, {e1, e3} , {e1,e2} };

		EXPECT_EQ(p, a);
		EXPECT_EQ(out, b);
	}

	TEST_F(UtilityTest, getEntitiesWithoutDuplicate) {
		Entity e1 = { STMT,Synonym{"e1"} };
		Entity e2 = { STMT,Synonym{"e2"} };
		Entity e3 = { STMT,Synonym{"e3"} };
		Entity e4 = { STMT,Synonym{"e4"} };
		Entity e5 = { STMT,Synonym{"e5"} };

		std::vector<Entity> a = { e1, e2, e1, e3, e2, e3, e4, e5, e1, e2, e3, e4, e5 };
		std::vector<Entity> b = { e1, e2, e3, e4, e5 };
		EXPECT_EQ(Utility::getEntitiesWithoutDuplicate(a), b);
	}

	TEST_F(UtilityTest, isSecondaryAttribute) {
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

	TEST_F(UtilityTest, getColumnsWithoutDuplicate) {
		std::list<std::vector<value>> a = {
			{1,1,1},
			{1,2,1},
			{1,3,1} };

		std::vector<int> b = { 1, 2 };
		std::list<std::vector<value>> to = { {1,1}, {2,1}, {3,1} };

		std::list<std::vector<value>> out;
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);

		b = { 0, 2 };
		to = { {1,1} };
		out.clear();
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);

		b = { 0, 1 };
		to = { {1,1}, {1,2}, {1,3} };
		out.clear();
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);

		b = { 0 };
		to = { {1} };
		out.clear();
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);

		b = { 1 };
		to = { {1}, {2}, {3} };
		out.clear();
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);

		b = { 2 };
		to = { {1} };
		out.clear();
		Utility::getColumnsWithoutDuplicate(a, b, out);
		EXPECT_EQ(out, to);
	}
	TEST_F(UtilityTest, queryTokenTypeToEntityType) {
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