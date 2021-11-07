#include <iostream>

#include "pch.h"
#include "PKB/PKB.h"
#include "PKB/RelationPreprocessorManager.h"

namespace UnitTesting {
	class RelationPreprocessorManagerTest_NextT : public testing::Test {
	protected:
		RelationPreprocessorManagerTest_NextT() {
			PKB::getInstance().resetCache();

			for (auto& stmt : stmt_list) {
				PKB::getInstance().addStmt(stmt.stmt_type);
			}

			PKB::getInstance().addNext(1, 2);
			PKB::getInstance().addNext(2, 3);
			PKB::getInstance().addNext(3, 4);
			PKB::getInstance().addNext(3, 5);
			PKB::getInstance().addNext(4, 2);
			PKB::getInstance().addNext(5, 2);
			PKB::getInstance().addNext(2, 6);

			PKB::getInstance().addProcedure(first);
			PKB::getInstance().addProcedure(second);

			PKB::getInstance().addProcContains(first, 1);
			PKB::getInstance().addProcContains(first, 2);
			PKB::getInstance().addProcContains(first, 3);
			PKB::getInstance().addProcContains(first, 4);
			PKB::getInstance().addProcContains(first, 5);
			PKB::getInstance().addProcContains(first, 6);
			PKB::getInstance().addProcContains(second, 7);

			manager.reset();
		}

		RelationPreprocessorManager manager;

		StmtInfo s1{ 1, STMT_READ };
		StmtInfo s2{ 2, STMT_WHILE };
		StmtInfo s3{ 3, STMT_IF };
		StmtInfo s4{ 4, STMT_PRINT };
		StmtInfo s5{ 5, STMT_READ };
		StmtInfo s6{ 6, STMT_READ };
		StmtInfo s7{ 7, STMT_READ };
		std::vector<StmtInfo> stmt_list = { s1, s2, s3, s4, s5, s6, s7 };

		proc_name first = "first";
		proc_name second = "second";

		std::vector<StmtInfo> empty_result = {};
		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = {
			{s1, s2}, {s1, s3}, {s1, s4}, {s1, s5}, {s1, s6},
			{s2, s2}, {s2, s3}, {s2, s4}, {s2, s5}, {s2, s6},
			{s3, s2}, {s3, s3}, {s3, s4}, {s3, s5}, {s3, s6},
			{s4, s2}, {s4, s3}, {s4, s4}, {s4, s5}, {s4, s6},
			{s5, s2}, {s5, s3}, {s5, s4}, {s5, s5}, {s5, s6},
		};

		void TearDown() override {
			manager.reset();
			PKB::getInstance().resetCache();
		}
	};

	TEST_F(RelationPreprocessorManagerTest_NextT, isNextTEmpty) {
		EXPECT_FALSE(manager.isNextTEmpty());
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, isPreviousT) {
		EXPECT_TRUE(manager.isPreviousT(1));
		EXPECT_TRUE(manager.isPreviousT(2));
		EXPECT_TRUE(manager.isPreviousT(3));
		EXPECT_TRUE(manager.isPreviousT(4));
		EXPECT_TRUE(manager.isPreviousT(5));
		EXPECT_FALSE(manager.isPreviousT(6));
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, isNextT_identifier) {
		EXPECT_FALSE(manager.isNextT(1));
		EXPECT_TRUE(manager.isNextT(2));
		EXPECT_TRUE(manager.isNextT(3));
		EXPECT_TRUE(manager.isNextT(4));
		EXPECT_TRUE(manager.isNextT(5));
		EXPECT_TRUE(manager.isNextT(6));
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, isNextT_all) {
		EXPECT_FALSE(manager.isNextT(1, 1));
		EXPECT_TRUE(manager.isNextT(1, 2));
		EXPECT_TRUE(manager.isNextT(1, 3));
		EXPECT_TRUE(manager.isNextT(1, 4));
		EXPECT_TRUE(manager.isNextT(1, 5));
		EXPECT_TRUE(manager.isNextT(1, 6));

		EXPECT_FALSE(manager.isNextT(2, 1));
		EXPECT_TRUE(manager.isNextT(2, 2));
		EXPECT_TRUE(manager.isNextT(2, 3));
		EXPECT_TRUE(manager.isNextT(2, 4));
		EXPECT_TRUE(manager.isNextT(2, 5));
		EXPECT_TRUE(manager.isNextT(2, 6));

		EXPECT_FALSE(manager.isNextT(3, 1));
		EXPECT_TRUE(manager.isNextT(3, 2));
		EXPECT_TRUE(manager.isNextT(3, 3));
		EXPECT_TRUE(manager.isNextT(3, 4));
		EXPECT_TRUE(manager.isNextT(3, 5));
		EXPECT_TRUE(manager.isNextT(3, 6));

		EXPECT_FALSE(manager.isNextT(4, 1));
		EXPECT_TRUE(manager.isNextT(4, 2));
		EXPECT_TRUE(manager.isNextT(4, 3));
		EXPECT_TRUE(manager.isNextT(4, 4));
		EXPECT_TRUE(manager.isNextT(4, 5));
		EXPECT_TRUE(manager.isNextT(4, 6));

		EXPECT_FALSE(manager.isNextT(5, 1));
		EXPECT_TRUE(manager.isNextT(5, 2));
		EXPECT_TRUE(manager.isNextT(5, 3));
		EXPECT_TRUE(manager.isNextT(5, 4));
		EXPECT_TRUE(manager.isNextT(5, 5));
		EXPECT_TRUE(manager.isNextT(5, 6));

		EXPECT_FALSE(manager.isNextT(6, 1));
		EXPECT_FALSE(manager.isNextT(6, 2));
		EXPECT_FALSE(manager.isNextT(6, 3));
		EXPECT_FALSE(manager.isNextT(6, 4));
		EXPECT_FALSE(manager.isNextT(6, 5));
		EXPECT_FALSE(manager.isNextT(6, 6));

		EXPECT_FALSE(manager.isNextT(1, 1));
		EXPECT_TRUE(manager.isNextT(1, 2));
		EXPECT_TRUE(manager.isNextT(1, 3));
		EXPECT_TRUE(manager.isNextT(1, 4));
		EXPECT_TRUE(manager.isNextT(1, 5));
		EXPECT_TRUE(manager.isNextT(1, 6));
	}
	TEST_F(RelationPreprocessorManagerTest_NextT, getAllNextTRelation) {
		auto v1 = manager.getAllNextTRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, getNextT_double_identifier) {
		auto v1 = manager.getNextT();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, getPreviousT_all) {
		auto v1 = manager.getPreviousT();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, getNextT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getNextT(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.first == stmt) {
					v2.push_back(pair.second);
				}
			}
			EXPECT_EQ(v1, v2);
		}
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, getPreviousT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getPreviousT(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
		}
	}

	TEST_F(RelationPreprocessorManagerTest_NextT, reset) {
		manager.getAllNextTRelation();
		EXPECT_TRUE(manager.getNextTPreprocessor().isFullyPopulated());
		EXPECT_FALSE(manager.getNextTPreprocessor().isCacheEmpty());
		manager.reset();
		EXPECT_FALSE(manager.getNextTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getNextTPreprocessor().isCacheEmpty());
	}
}