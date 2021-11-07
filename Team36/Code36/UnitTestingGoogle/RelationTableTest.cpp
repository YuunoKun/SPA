#include "pch.h"

#include "PKB/RelationTables/RelationTable.h"
#include "PKB/RelationTables/RelationTable.cpp"
#include "Common/Common.h"

namespace UnitTesting {
	TEST(RelationTable, equal) {
		RelationTable<int, int> table1, table2, table3;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1, table1);
		EXPECT_EQ(table2, table2);
		EXPECT_EQ(table3, table3);

		EXPECT_EQ(table1, table2);
		EXPECT_NE(table1, table3);
		EXPECT_NE(table2, table3);
	}
	TEST(RelationTable, insert) {
		RelationTable<int, int> table;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };

		EXPECT_EQ(table.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table.insert(testInput[4].first, testInput[4].second), true);
	}

	TEST(RelationTable, insert_StmtInfo) {
		RelationTable<int, StmtInfo> table1, table2;
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_READ };
		StmtInfo p3{ 3, STMT_READ };

		std::vector<std::pair<int, StmtInfo>> testInput{ {1, p2}, {1, p3}, {2, p2}, {2, p2}, {2, p3} };

		int key = testInput[0].first;
		StmtInfo value = testInput[0].second;

		EXPECT_EQ(table2.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table2.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table2.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table2.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table2.insert(testInput[4].first, testInput[4].second), true);
	}

	TEST(RelationTable, getKeysAll) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2,3} };
		std::vector<int> expectedKeys{ 1, 2 };
		std::vector<int> blankKeys{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), blankKeys);
	}

	TEST(RelationTable, getKeysAll_StmtInfo) {
		RelationTable<StmtInfo, StmtInfo> table1, table2;
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_READ };
		StmtInfo p3{ 3, STMT_READ };

		std::vector<std::pair<StmtInfo, StmtInfo>> testInput{ {p1, p2}, {p1, p3}, {p2, p2}, {p2, p2}, {p2, p3} };
		std::vector<StmtInfo> expectedKeys{ p1, p2 };
		std::vector<StmtInfo> blankKeys{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), blankKeys);
	}

	TEST(RelationTable, getValuesAll) {
		RelationTable<int, int> table1;
		RelationTable<int, int> table2, table3;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };

		std::vector<int> expectedValues{ 2, 3 };

		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getValues(), expectedValues);
		EXPECT_EQ(table2.getValues(), blankValues);
	}

	TEST(RelationTable, getKeys) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValues{ 1, 2 };

		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getKeys(1), blankValues);
		EXPECT_EQ(table1.getKeys(2), expectedValues);
		EXPECT_EQ(table1.getKeys(3), expectedValues);
		EXPECT_EQ(table2.getKeys(1), blankValues);
		EXPECT_EQ(table2.getKeys(2), blankValues);
		EXPECT_EQ(table2.getKeys(3), blankValues);
	}

	TEST(RelationTable, getValues) {
		RelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValues{ 2, 3 };

		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getValues(1), expectedValues);
		EXPECT_EQ(table1.getValues(2), expectedValues);
		EXPECT_EQ(table1.getValues(3), blankValues);
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
		RelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1.containsKey(1), true);
		EXPECT_EQ(table1.containsKey(2), false);
	}

	TEST(RelationTable, containsValue) {
		RelationTable<int, var_name> table;
		std::vector<std::pair<int, var_name>> testInput2{ {1, "x"} };

		table.insert(testInput2[0].first, testInput2[0].second);

		EXPECT_EQ(table.containsValue("x"), true);
		EXPECT_EQ(table.containsValue("y"), false);
	}

	TEST(RelationTable, containsPair) {
		RelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> blankValues{ };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.containsPair(1, 2), true);
		EXPECT_EQ(table1.containsPair(2, 3), true);
		EXPECT_EQ(table1.containsPair(2, 2), true);
		EXPECT_EQ(table1.containsPair(2, 1), false);
		EXPECT_EQ(table1.containsPair(3, 1), false);
		EXPECT_EQ(table1.containsPair(0, 1), false);
	}

	TEST(RelationTable, clear) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		EXPECT_NE(table1, table2);
		table1.clear();
		EXPECT_EQ(table1, table2);
	}

	TEST(RelationTable, isEmpty) {
		RelationTable<int, int> table1, table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);
		EXPECT_FALSE(table1.isEmpty());
		EXPECT_TRUE(table2.isEmpty());
		table1.clear();
		EXPECT_TRUE(table1.isEmpty());
	}
}