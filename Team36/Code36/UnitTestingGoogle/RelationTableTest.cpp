#include "pch.h"

#include "RelationTable.h"
#include "RelationTable.cpp"
#include "Common.h"

namespace UnitTesting {
	TEST(RelationTable, isUniqueKey) {
		UniqueRelationTable<int, int> table1;
		EXPECT_EQ(table1.isUniqueKey(), true);

		RelationTable<int, int> table2;
		EXPECT_EQ(table2.isUniqueKey(), false);
	}

	TEST(RelationTable, equal) {
		UniqueRelationTable<int, int> table1, table2;
		RelationTable<int, int> table3, table4;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		table3.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1, table1);
		EXPECT_EQ(table2, table2);
		EXPECT_EQ(table3, table3);
		EXPECT_EQ(table4, table4);

		EXPECT_NE(table1, table2);
		EXPECT_NE(table1, table3);
		EXPECT_NE(table1, table4);
		EXPECT_NE(table2, table3);
		EXPECT_NE(table2, table4);
		EXPECT_NE(table3, table4);
	}
	TEST(RelationTable, insert) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), false);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);

		EXPECT_EQ(table2.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table2.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table2.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table2.insert(testInput[3].first, testInput[3].second), false);
	}

	TEST(RelationTable, insert_StmtInfo) {
		UniqueRelationTable<StmtInfo, StmtInfo> table1;
		RelationTable<StmtInfo, StmtInfo> table2;
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_READ };
		StmtInfo p3{ 3, STMT_READ };

		std::vector<std::pair<StmtInfo, StmtInfo>> testInput{ {p1, p2}, {p1, p3}, {p2, p2}, {p2, p2} };

		StmtInfo key = testInput[0].first;
		StmtInfo value = testInput[0].second;

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), false);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);

		EXPECT_EQ(table2.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table2.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table2.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table2.insert(testInput[3].first, testInput[3].second), false);
	}

	TEST(RelationTable, getKeys) {
		UniqueRelationTable<int, int> table1, table3;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };
		std::vector<int> expectedKeys{ 1, 2 };
		std::vector<int> blankKeys{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);

		table2.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[1].first, testInput[1].second);
		table2.insert(testInput[2].first, testInput[2].second);
		table2.insert(testInput[3].first, testInput[3].second);

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), expectedKeys);
		EXPECT_EQ(table3.getKeys(), blankKeys);
	}

	TEST(RelationTable, getKeys_StmtInfo) {
		UniqueRelationTable<StmtInfo, StmtInfo> table1, table3;
		RelationTable<StmtInfo, StmtInfo> table2;
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_READ };
		StmtInfo p3{ 3, STMT_READ };

		std::vector<std::pair<StmtInfo, StmtInfo>> testInput{ {p1, p2}, {p1, p3}, {p2, p2}, {p2, p2} };
		std::vector<StmtInfo> expectedKeys{ p1, p2 };
		std::vector<StmtInfo> blankKeys{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);

		table2.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[1].first, testInput[1].second);
		table2.insert(testInput[2].first, testInput[2].second);
		table2.insert(testInput[3].first, testInput[3].second);

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), expectedKeys);
		EXPECT_EQ(table3.getKeys(), blankKeys);
	}

	TEST(RelationTable, getValues) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValuesOne{ 2 };
		std::vector<int> expectedValuesTwo{ 2, 3 };
		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		table2.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[1].first, testInput[1].second);
		table2.insert(testInput[2].first, testInput[2].second);
		table2.insert(testInput[3].first, testInput[3].second);
		table2.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getValues(1), expectedValuesOne);
		EXPECT_EQ(table1.getValues(2), expectedValuesOne);
		EXPECT_EQ(table2.getValues(1), expectedValuesTwo);
		EXPECT_EQ(table2.getValues(2), expectedValuesTwo);
		EXPECT_EQ(table2.getValues(3), blankValues);
	}

	TEST(RelationTable, getPairs) {
		RelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };
		std::vector<std::pair<int, int>> expectedPairs{ std::make_pair(1, 2), std::make_pair(1, 3), std::make_pair(2, 2) };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);

		EXPECT_EQ(table1.getPairs(), expectedPairs);
	}

	TEST(RelationTable, containsKey) {
		UniqueRelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1.containsKey(1), true);
		EXPECT_EQ(table1.containsKey(2), false);
	}

	TEST(RelationTable, containsPair) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValuesOne{ 2 };
		std::vector<int> expectedValuesTwo{ 2, 3 };
		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		table2.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[1].first, testInput[1].second);
		table2.insert(testInput[2].first, testInput[2].second);
		table2.insert(testInput[3].first, testInput[3].second);
		table2.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.containsPair(1, 2), true);
		EXPECT_EQ(table1.containsPair(2, 2), true);
		EXPECT_EQ(table1.containsPair(1, 3), false);
		EXPECT_EQ(table1.containsPair(2, 1), false);
		EXPECT_EQ(table1.containsPair(3, 1), false);
		EXPECT_EQ(table1.containsPair(0, 1), false);

		EXPECT_EQ(table1.containsPair(1, 2), true);
		EXPECT_EQ(table1.containsPair(2, 2), true);
		EXPECT_EQ(table1.containsPair(2, 1), false);
		EXPECT_EQ(table1.containsPair(3, 1), false);
		EXPECT_EQ(table1.containsPair(0, 1), false);
	}

	TEST(RelationTable, reverse) {
		RelationTable<int, int> table1, table2, reversed_table1, reversed_table3;
		UniqueRelationTable<int, int> table3;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 4} };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		table3.insert(testInput[0].first, testInput[0].second);
		table3.insert(testInput[1].first, testInput[1].second);
		table3.insert(testInput[2].first, testInput[2].second);
		table3.insert(testInput[3].first, testInput[3].second);
		table3.insert(testInput[4].first, testInput[4].second);

		reversed_table1.insert(testInput[0].second, testInput[0].first);
		reversed_table1.insert(testInput[1].second, testInput[1].first);
		reversed_table1.insert(testInput[2].second, testInput[2].first);
		reversed_table1.insert(testInput[3].second, testInput[3].first);
		reversed_table1.insert(testInput[4].second, testInput[4].first);

		reversed_table3.insert(testInput[0].second, testInput[0].first);
		reversed_table3.insert(testInput[2].second, testInput[2].first);

		EXPECT_EQ(table1.findReverse(), reversed_table1);
		EXPECT_NE(table1.findReverse(), table1);
		EXPECT_NE(table1.findReverse(), table2);
		EXPECT_EQ(table3.findReverse(), reversed_table3);
	}

	TEST(RelationTable, clear) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		EXPECT_NE(table1, table2);
		table1.clear();
		EXPECT_EQ(table1, table2);
	}

	TEST(RelationTable, empty) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		EXPECT_FALSE(table1.empty());
		EXPECT_TRUE(table2.empty());
		table1.clear();
		EXPECT_TRUE(table1.empty());
	}
}