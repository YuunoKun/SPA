#include "pch.h"

#include "Common/TransitiveClosureHelper.h"

namespace UnitTesting {
	TEST(TransitiveClosureHelperTest, findTransitiveClosure) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {2, 4}, {3, 3}, {3, 4}, {4, 5} };
		std::vector<std::pair<int, int>> testExpected{ {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 3}, {2, 4}, {2, 5}, {3, 3}, {3, 4}, {3, 5}, {4, 5} };

		auto test = TransitiveClosureHelper<int>::findTransitiveClosure(testInput);
		EXPECT_EQ(test, testExpected);
	}

	TEST(TransitiveClosureHelperTest, findTransitiveClosureCycle) {
		std::vector<std::pair<int, int>> testInput{ {1, 2}, {2, 3}, {3,1} };
		std::vector<std::pair<int, int>> testExpected{ {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3} };

		auto test = TransitiveClosureHelper<int>::findTransitiveClosure(testInput);
		EXPECT_EQ(test, testExpected);
	}
}