#include "pch.h"

#include "MonotypeRelationTable.h"
#include "MonotypeRelationTable.cpp"
#include "Common.h"

namespace UnitTesting {
	TEST(MonotypeRelationTable, findTransitiveClosure) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {4, 5} };
		std::vector<std::pair<int, int>> testExpected{ {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 5} };

		MonotypeRelationTable<int> table{ testInput }, transitive_table{ testExpected };

		auto test1 = table.findTransitiveClosure();
		EXPECT_EQ(test1, transitive_table);
	}

	TEST(MonotypeRelationTable, findTransitiveClosureCycle) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {3,1} };
		std::vector<std::pair<int, int>> testExpected{ {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3} };
		MonotypeRelationTable<int> table{ testInput }, transitive_table{ testExpected };

		auto test2 = table.findTransitiveClosure();
		EXPECT_EQ(test2, transitive_table);
	}

	TEST(MonotypeRelationTable, copy) {
		MonotypeRelationTable<int> table, table_empty;
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {3,1} };
		for (auto& it : testInput) {
			table.insert(it.first, it.second);
		}

		auto table_copy = table.copy();
		auto table_empty_copy = table_empty.copy();

		EXPECT_EQ(table, table_copy);
		EXPECT_EQ(table_empty, table_empty_copy);
	}

	TEST(MonotypeRelationTable, forwardDFS) {
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
		std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
		std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

		std::vector<int> expected1{ {1, 2, 3} };

		MonotypeRelationTable<int> table1{ testInput1 }, table2{ testInput2 }, table3{ testInput3 };

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

	TEST(MonotypeRelationTable, backwardDFS) {
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
		std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
		std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
			{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

		std::vector<int> expected1{ {1, 2, 3} };

		MonotypeRelationTable<int> table1{ testInput1 }, table2{ testInput2 }, table3{ testInput3 };

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
}