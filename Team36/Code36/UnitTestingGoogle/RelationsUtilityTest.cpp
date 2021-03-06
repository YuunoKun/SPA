#include "pch.h"

#include "PKB/RelationTables/MonotypeRelationTable.h"
#include "Common/Common.h"
#include "PKB/RelationTables/RelationTableUtility.cpp"

namespace UnitTesting {
	TEST(RelationTableUtilityTest, findTransitiveClosure) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {4, 5} };
		std::vector<std::pair<int, int>> testExpected{ {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 5} };
		MonotypeRelationTable<int> table{ testInput };
		MonotypeRelationTable<int> transitive_table{ testExpected };

		auto test = RelationTableUtility<int>::findTransitiveClosure(table);
		EXPECT_EQ(test, transitive_table);
	}

	TEST(RelationTableUtilityTest, findTransitiveClosureCycle) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {3,1} };
		std::vector<std::pair<int, int>> testExpected{ {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3} };
		MonotypeRelationTable<int> table{ testInput }, transitive_table{ testExpected };

		auto test2 = RelationTableUtility<int>::findTransitiveClosure(table);
		EXPECT_EQ(test2, transitive_table);
	}

	TEST(RelationTableUtilityTest, forwardDFS) {
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
			v1 = RelationTableUtility<int>::forwardDFS(table1, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected1);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = RelationTableUtility<int>::forwardDFS(table2, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected2[i - 1]);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = RelationTableUtility<int>::forwardDFS(table3, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected3[i - 1]);
		}
	}

	TEST(RelationTableUtilityTest, backwardDFS) {
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
			v1 = RelationTableUtility<int>::backwardDFS(table1, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected1);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = RelationTableUtility<int>::backwardDFS(table2, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected2[i - 1]);
		}

		for (int i = 1; i <= 8; i++) {
			v1 = RelationTableUtility<int>::backwardDFS(table3, i);
			std::sort(v1.begin(), v1.end());
			EXPECT_EQ(v1, expected3[i - 1]);
		}
	}
}