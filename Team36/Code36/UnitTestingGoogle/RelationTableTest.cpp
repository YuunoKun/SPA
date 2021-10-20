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
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), false);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table1.insert(testInput[4].first, testInput[4].second), false);

		EXPECT_EQ(table2.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table2.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table2.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table2.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table2.insert(testInput[4].first, testInput[4].second), true);
	}

	TEST(RelationTable, insert_StmtInfo) {
		UniqueRelationTable<int, StmtInfo> table1;
		RelationTable<int, StmtInfo> table2;
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_READ };
		StmtInfo p3{ 3, STMT_READ };

		std::vector<std::pair<int, StmtInfo>> testInput{ {1, p2}, {1, p3}, {2, p2}, {2, p2}, {2, p3} };

		int key = testInput[0].first;
		StmtInfo value = testInput[0].second;

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), false);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table1.insert(testInput[4].first, testInput[4].second), false);

		EXPECT_EQ(table2.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table2.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table2.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table2.insert(testInput[3].first, testInput[3].second), false);
		EXPECT_EQ(table2.insert(testInput[4].first, testInput[4].second), true);
	}

	TEST(RelationTable, getKeysAll) {
		UniqueRelationTable<int, int> table1, table3;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2,3} };
		std::vector<int> expectedKeys{ 1, 2 };
		std::vector<int> blankKeys{ };

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

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), expectedKeys);
		EXPECT_EQ(table3.getKeys(), blankKeys);
	}

	TEST(RelationTable, getKeysAll_StmtInfo) {
		UniqueRelationTable<StmtInfo, StmtInfo> table1, table3;
		RelationTable<StmtInfo, StmtInfo> table2;
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

		table2.insert(testInput[0].first, testInput[0].second);
		table2.insert(testInput[1].first, testInput[1].second);
		table2.insert(testInput[2].first, testInput[2].second);
		table2.insert(testInput[3].first, testInput[3].second);
		table2.insert(testInput[4].first, testInput[4].second);

		EXPECT_EQ(table1.getKeys(), expectedKeys);
		EXPECT_EQ(table2.getKeys(), expectedKeys);
		EXPECT_EQ(table3.getKeys(), blankKeys);
	}

	TEST(RelationTable, getValuesAll) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2, table3;
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

		EXPECT_EQ(table1.getValues(), expectedValuesOne);
		EXPECT_EQ(table2.getValues(), expectedValuesTwo);
		EXPECT_EQ(table3.getValues(), blankValues);
	}

	TEST(RelationTable, getKeys) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValuesOne{ 1 };
		std::vector<int> expectedValuesTwo{ 2 };
		std::vector<int> expectedValuesThree{ 1, 2 };

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

		EXPECT_EQ(table1.getKeys(1), blankValues);
		EXPECT_EQ(table1.getKeys(2), expectedValuesThree);
		EXPECT_EQ(table1.getKeys(3), blankValues);
		EXPECT_EQ(table2.getKeys(1), blankValues);
		EXPECT_EQ(table2.getKeys(2), expectedValuesThree);
		EXPECT_EQ(table2.getKeys(3), expectedValuesThree);
	}

	TEST(RelationTable, getValues) {
		UniqueRelationTable<int, int> table1;
		RelationTable<int, int> table2;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<int> expectedValuesOne{ 2 };
		std::vector<int> expectedValuesTwo{ 2, 3 };
		std::vector<int> expectedValuesThree{ 3 };

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

	TEST(RelationTable, containsValue) {
		UniqueRelationTable<int, int> table1;
		std::vector<std::pair<int, int>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1.containsValue(2), true);
		EXPECT_EQ(table1.containsValue(1), false);

		UniqueRelationTable<int, var_name> table2;
		std::vector<std::pair<int, var_name>> testInput2{ {1, "x"} };

		table2.insert(testInput2[0].first, testInput2[0].second);

		EXPECT_EQ(table2.containsValue("x"), true);
		EXPECT_EQ(table2.containsValue("y"), false);
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
		EXPECT_EQ(table1.containsPair(2, 3), false);
		EXPECT_EQ(table1.containsPair(2, 2), true);
		EXPECT_EQ(table1.containsPair(1, 3), false);
		EXPECT_EQ(table1.containsPair(2, 1), false);
		EXPECT_EQ(table1.containsPair(3, 1), false);
		EXPECT_EQ(table1.containsPair(0, 1), false);

		EXPECT_EQ(table2.containsPair(1, 2), true);
		EXPECT_EQ(table2.containsPair(2, 3), true);
		EXPECT_EQ(table2.containsPair(2, 2), true);
		EXPECT_EQ(table2.containsPair(2, 1), false);
		EXPECT_EQ(table2.containsPair(3, 1), false);
		EXPECT_EQ(table2.containsPair(0, 1), false);
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

	TEST(RelationTable, findTransitiveClosure) {
		RelationTable<int, int> table1, transitive_table1;
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {4, 5} };
		std::vector<std::pair<int, int>> testExpected1{ {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 5} };
		table1.insert(testInput1[0].first, testInput1[0].second);
		table1.insert(testInput1[1].first, testInput1[1].second);
		table1.insert(testInput1[2].first, testInput1[2].second);
		table1.insert(testInput1[3].first, testInput1[3].second);
		table1.insert(testInput1[4].first, testInput1[4].second);
		table1.insert(testInput1[5].first, testInput1[5].second);

		transitive_table1.insert(testExpected1[0].first, testExpected1[0].second);
		transitive_table1.insert(testExpected1[1].first, testExpected1[1].second);
		transitive_table1.insert(testExpected1[2].first, testExpected1[2].second);
		transitive_table1.insert(testExpected1[3].first, testExpected1[3].second);
		transitive_table1.insert(testExpected1[4].first, testExpected1[4].second);
		transitive_table1.insert(testExpected1[5].first, testExpected1[5].second);
		transitive_table1.insert(testExpected1[6].first, testExpected1[6].second);
		transitive_table1.insert(testExpected1[7].first, testExpected1[7].second);
		transitive_table1.insert(testExpected1[8].first, testExpected1[8].second);
		transitive_table1.insert(testExpected1[9].first, testExpected1[9].second);
		transitive_table1.insert(testExpected1[10].first, testExpected1[10].second);

		auto test1 = table1.findTransitiveClosure();
		EXPECT_EQ(test1, transitive_table1);
	}

	TEST(RelationTable, findTransitiveClosureCycle) {
		RelationTable<int, int> table, transitive_table;
		std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {3,1} };
		std::vector<std::pair<int, int>> testExpected2{ {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3} };
		table.insert(testInput2[0].first, testInput2[0].second);
		table.insert(testInput2[1].first, testInput2[1].second);
		table.insert(testInput2[2].first, testInput2[2].second);

		transitive_table.insert(testExpected2[0].first, testExpected2[0].second);
		transitive_table.insert(testExpected2[1].first, testExpected2[1].second);
		transitive_table.insert(testExpected2[2].first, testExpected2[2].second);
		transitive_table.insert(testExpected2[3].first, testExpected2[3].second);
		transitive_table.insert(testExpected2[4].first, testExpected2[4].second);
		transitive_table.insert(testExpected2[5].first, testExpected2[5].second);
		transitive_table.insert(testExpected2[6].first, testExpected2[6].second);
		transitive_table.insert(testExpected2[7].first, testExpected2[7].second);
		transitive_table.insert(testExpected2[8].first, testExpected2[8].second);

		auto test2 = table.findTransitiveClosure();
		EXPECT_EQ(test2, transitive_table);
	}

	TEST(RelationTable, copy) {
		RelationTable<int, int> table, table_empty;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {3,1} };
		for (auto& it : testInput) {
			table.insert(it.first, it.second);
		}

		auto table_copy = table.copy();
		auto table_empty_copy = table_empty.copy();

		EXPECT_EQ(table, table_copy);
		EXPECT_EQ(table_empty, table_empty_copy);
	}

	TEST(RelationTable, forwardDFS) {
		RelationTable<int, int> table1, table2, table3;
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
		std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
		std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

		std::vector<int> expected1{ {1, 2, 3} };
		std::vector<std::vector<int>> expected2{
			{2, 3, 4, 5, 6, 7, 8},
			{3, 4, 5, 6, 7, 8},
			{5, 6, 7, 8},
			{5, 6, 7, 8},
			{6, 7, 8},
			{6, 7},
			{6, 7},
			{}
		};
		std::vector<std::vector<int>> expected3{
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{1, 2, 3, 4, 5, 6, 7, 8},
			{}
		};

		table1.insert(testInput1[0].first, testInput1[0].second);
		table1.insert(testInput1[1].first, testInput1[1].second);
		table1.insert(testInput1[2].first, testInput1[2].second);

		for (auto& it : testInput2) {
			table2.insert(it.first, it.second);
		}
		for (auto& it : testInput3) {
			table3.insert(it.first, it.second);
		}

		std::vector<int> v1;
		for (int i = 1; i <= 3; i++) {
			v1 = table1.forwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected1);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = table2.forwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected2[i - 1]);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = table3.forwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected3[i - 1]);
		}
	}

	TEST(RelationTable, backwardDFS) {
		RelationTable<int, int> table1, table2, table3;
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
		std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
		std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

		std::vector<int> expected1{ {1, 2, 3} };
		std::vector<std::vector<int>> expected2{
			{},
			{1},
			{1,2},
			{1,2},
			{1,2,3,4},
			{1,2,3,4,5,6,7},
			{1,2,3,4,5,6,7},
			{1,2,3,4,5}
		};
		std::vector<std::vector<int>> expected3{
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7},
			{1, 2, 3, 4, 5, 6, 7}
		};

		for (auto& it : testInput1) {
			table1.insert(it.first, it.second);
		}
		for (auto& it : testInput2) {
			table2.insert(it.first, it.second);
		}
		for (auto& it : testInput3) {
			table3.insert(it.first, it.second);
		}

		std::vector<int> v1;
		for (int i = 1; i <= 3; i++) {
			v1 = table1.backwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected1);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = table2.backwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected2[i - 1]);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = table3.backwardDFS(i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected3[i - 1]);
		}
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