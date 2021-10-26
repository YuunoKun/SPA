#include "pch.h"

#include "RelationTable.h"
#include "RelationTable.cpp"
#include "Common.h"
#include "RelationsUtility.cpp"

namespace UnitTesting {
	TEST(RelationTable, findTransitiveClosure) {
		std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {4, 5} };
		std::vector<std::pair<int, int>> testExpected1{ {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 5} };
		RelationTable<int, int> table1{ testInput1 };
		RelationTable<int, int> transitive_table1{ testExpected1 };

		auto test1 = RelationsUtility<int>::findTransitiveClosure(table1);
		EXPECT_EQ(test1, transitive_table1);
	}

	//TEST(RelationTable, findTransitiveClosureCycle) {
	//	RelationTable<int, int> table, transitive_table;
	//	std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {3,1} };
	//	std::vector<std::pair<int, int>> testExpected2{ {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3} };
	//	table.insert(testInput2[0].first, testInput2[0].second);
	//	table.insert(testInput2[1].first, testInput2[1].second);
	//	table.insert(testInput2[2].first, testInput2[2].second);

	//	transitive_table.insert(testExpected2[0].first, testExpected2[0].second);
	//	transitive_table.insert(testExpected2[1].first, testExpected2[1].second);
	//	transitive_table.insert(testExpected2[2].first, testExpected2[2].second);
	//	transitive_table.insert(testExpected2[3].first, testExpected2[3].second);
	//	transitive_table.insert(testExpected2[4].first, testExpected2[4].second);
	//	transitive_table.insert(testExpected2[5].first, testExpected2[5].second);
	//	transitive_table.insert(testExpected2[6].first, testExpected2[6].second);
	//	transitive_table.insert(testExpected2[7].first, testExpected2[7].second);
	//	transitive_table.insert(testExpected2[8].first, testExpected2[8].second);

	//	auto test2 = table.findTransitiveClosure();
	//	EXPECT_EQ(test2, transitive_table);
	//}

	//TEST(RelationTable, forwardDFS) {
	//	RelationTable<int, int> table1, table2, table3;
	//	std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
	//	std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
	//		{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
	//	std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
	//		{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

	//	std::vector<int> expected1{ {1, 2, 3} };
	//	std::vector<std::vector<int>> expected2{
	//		{2, 3, 4, 5, 6, 7, 8},
	//		{3, 4, 5, 6, 7, 8},
	//		{5, 6, 7, 8},
	//		{5, 6, 7, 8},
	//		{6, 7, 8},
	//		{6, 7},
	//		{6, 7},
	//		{}
	//	};
	//	std::vector<std::vector<int>> expected3{
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{1, 2, 3, 4, 5, 6, 7, 8},
	//		{}
	//	};

	//	table1.insert(testInput1[0].first, testInput1[0].second);
	//	table1.insert(testInput1[1].first, testInput1[1].second);
	//	table1.insert(testInput1[2].first, testInput1[2].second);

	//	for (auto& it : testInput2) {
	//		table2.insert(it.first, it.second);
	//	}
	//	for (auto& it : testInput3) {
	//		table3.insert(it.first, it.second);
	//	}

	//	std::vector<int> v1;
	//	for (int i = 1; i <= 3; i++) {
	//		v1 = table1.forwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected1);
	//	}

	//	for (int i = 1; i <= 8; i++) {
	//		v1 = table2.forwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected2[i - 1]);
	//	}

	//	for (int i = 1; i <= 8; i++) {
	//		v1 = table3.forwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected3[i - 1]);
	//	}
	//}

	//TEST(RelationTable, backwardDFS) {
	//	RelationTable<int, int> table1, table2, table3;
	//	std::vector<std::pair<int, int>> testInput1{ {1, 2}, {2, 3}, {3, 1} };
	//	std::vector<std::pair<int, int>> testInput2{ {1, 2}, {2, 3}, {2, 4},
	//		{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6} };
	//	std::vector<std::pair<int, int>> testInput3{ {1, 2}, {2, 3}, {2, 4},
	//		{3, 5}, {4,5}, {5, 6}, {5, 8}, {6,7}, {7,6}, {7,1} };

	//	std::vector<int> expected1{ {1, 2, 3} };
	//	std::vector<std::vector<int>> expected2{
	//		{},
	//		{1},
	//		{1,2},
	//		{1,2},
	//		{1,2,3,4},
	//		{1,2,3,4,5,6,7},
	//		{1,2,3,4,5,6,7},
	//		{1,2,3,4,5}
	//	};
	//	std::vector<std::vector<int>> expected3{
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7},
	//		{1, 2, 3, 4, 5, 6, 7}
	//	};

	//	for (auto& it : testInput1) {
	//		table1.insert(it.first, it.second);
	//	}
	//	for (auto& it : testInput2) {
	//		table2.insert(it.first, it.second);
	//	}
	//	for (auto& it : testInput3) {
	//		table3.insert(it.first, it.second);
	//	}

	//	std::vector<int> v1;
	//	for (int i = 1; i <= 3; i++) {
	//		v1 = table1.backwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected1);
	//	}

	//	for (int i = 1; i <= 8; i++) {
	//		v1 = table2.backwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected2[i - 1]);
	//	}

	//	for (int i = 1; i <= 8; i++) {
	//		v1 = table3.backwardDFS(i);
	//		std::sort(v1.begin(), v1.end());
	//		EXPECT_EQ(v1, expected3[i - 1]);
	//	}
	//}
}