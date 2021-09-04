#include "pch.h"

#include "RelationTable.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelationTable, isUniqueKey) {
		RelationTable table1 = RelationTable(true);
		EXPECT_EQ(table1.isUniqueKey(), true);

		RelationTable table2 = RelationTable(false);
		EXPECT_EQ(table2.isUniqueKey(), false);
	}

	TEST(RelationTable, insertUniqueKey) {
		RelationTable table1 = RelationTable(true);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), false);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);
	}

	TEST(RelationTable, insertNonUniqueKey) {
		RelationTable table1 = RelationTable(false);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };

		EXPECT_EQ(table1.insert(testInput[0].first, testInput[0].second), true);
		EXPECT_EQ(table1.insert(testInput[1].first, testInput[1].second), true);
		EXPECT_EQ(table1.insert(testInput[2].first, testInput[2].second), true);
		EXPECT_EQ(table1.insert(testInput[3].first, testInput[3].second), false);
	}

	TEST(RelationTable, insertInvalidArg) {
		RelationTable table1 = RelationTable(true);
		RelationTable table2 = RelationTable(false);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {0,0}, {1, 0}, {0, 1} };

		EXPECT_THROW(table1.insert(testInput[0].first, testInput[0].second), std::invalid_argument);
		EXPECT_THROW(table2.insert(testInput[0].first, testInput[0].second), std::invalid_argument);

		EXPECT_THROW(table1.insert(testInput[1].first, testInput[1].second), std::invalid_argument);
		EXPECT_THROW(table2.insert(testInput[1].first, testInput[1].second), std::invalid_argument);

		EXPECT_THROW(table1.insert(testInput[2].first, testInput[2].second), std::invalid_argument);
		EXPECT_THROW(table2.insert(testInput[2].first, testInput[2].second), std::invalid_argument);
	}

	TEST(RelationTable, getKeys) {
		RelationTable table1 = RelationTable(true);
		RelationTable table2 = RelationTable(false);
		RelationTable table3 = RelationTable(true);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2} };
		std::vector<stmt_index> expectedKeys{ 1, 2 };
		std::vector<stmt_index> blankKeys{ };

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
		RelationTable table1 = RelationTable(true);
		RelationTable table2 = RelationTable(false);
		RelationTable table3 = RelationTable(true);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<stmt_index> expectedValuesOne{ 2 };
		std::vector<stmt_index> expectedValuesTwo{ 2, 3 };
		std::vector<stmt_index> blankValues{ };

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

	TEST(RelationTable, containsKey) {
		RelationTable table1 = RelationTable(true);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2} };

		table1.insert(testInput[0].first, testInput[0].second);

		EXPECT_EQ(table1.containsKey(1), true);
		EXPECT_EQ(table1.containsKey(2), false);
	}

	TEST(RelationTable, containsPair) {
		RelationTable table1 = RelationTable(true);
		RelationTable table2 = RelationTable(false);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 3} };
		std::vector<stmt_index> expectedValuesOne{ 2 };
		std::vector<stmt_index> expectedValuesTwo{ 2, 3 };
		std::vector<stmt_index> blankValues{ };

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
		RelationTable table1 = RelationTable(false);
		RelationTable reversed_table1 = RelationTable(false);
		RelationTable table2 = RelationTable(false);
		RelationTable table3 = RelationTable(true);
		std::vector<std::pair<stmt_index, stmt_index>> testInput{ {1, 2}, {1, 3}, {2, 2}, {2, 2}, {2, 4} };

		table1.insert(testInput[0].first, testInput[0].second);
		table1.insert(testInput[1].first, testInput[1].second);
		table1.insert(testInput[2].first, testInput[2].second);
		table1.insert(testInput[3].first, testInput[3].second);
		table1.insert(testInput[4].first, testInput[4].second);

		reversed_table1.insert(testInput[0].second, testInput[0].first);
		reversed_table1.insert(testInput[1].second, testInput[1].first);
		reversed_table1.insert(testInput[2].second, testInput[2].first);
		reversed_table1.insert(testInput[3].second, testInput[3].first);
		reversed_table1.insert(testInput[4].second, testInput[4].first);

		EXPECT_EQ(table1.findReverse(), reversed_table1);
		EXPECT_NE(table1.findReverse(), table1);
		EXPECT_NE(table1.findReverse(), table2);
	}
}