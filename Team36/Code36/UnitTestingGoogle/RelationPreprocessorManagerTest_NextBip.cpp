#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "CFG.h"
#include "RelationPreprocessorManager.h"

namespace UnitTesting {
	class RelationPreprocessorManagerTest_NextBip : public testing::Test {
	protected:
		RelationPreprocessorManagerTest_NextBip() {
			PKB::getInstance().resetCache();

			cfg1->addLine(1);
			cfg1->addLine(2);
			cfg1->addLine(3);

			cfg2->addLine(4);
			cfg2->addLine(5);
			cfg2->addLine(6);
			cfg2->addLine(7);
			cfg2->addLine(8);

			cfg3->addLine(9);
			cfg3->addLine(10);
			cfg3->addLine(11);

			cfg4->addLine(12);

			cfg4_if->addLine(13);
			cfg4_if->addLine(14);

			cfg4_else->addLine(15);
			cfg4_else_addLoopAt->addLine(16);

			cfg4_else->addLoopAt(cfg4_else_addLoopAt, 15);
			cfg4->addForkAt(cfg4_if, cfg4_else, 12);
			cfg2->addCallAt(cfg3, 5);
			cfg2->addCallAt(cfg4, 7);
			cfg3->addCallAt(cfg4, 10);

			for (auto& stmt : stmt_list) {
				PKB::getInstance().addStmt(stmt.stmt_type);
			}

			PKB::getInstance().addCFGsToDestroy(test_cfg_bips);

			PKB::getInstance().addCFGBip(cfg1);
			PKB::getInstance().addCFGBip(cfg2);
			PKB::getInstance().addCFGBip(cfg3);
			PKB::getInstance().addCFGBip(cfg4);

			manager.reset();
		}

		RelationPreprocessorManager manager;

		CFG* cfg1 = new CFG();
		CFG* cfg2 = new CFG();
		CFG* cfg3 = new CFG();
		CFG* cfg4 = new CFG();
		CFG* cfg4_if = new CFG();
		CFG* cfg4_else = new CFG();
		CFG* cfg4_else_addLoopAt = new CFG();

		std::vector<CFG*> test_cfg_bips = { cfg1, cfg2, cfg3, cfg4, cfg4_if,
			cfg4_else, cfg4_else_addLoopAt
		};

		StmtInfo s1{ 1, STMT_ASSIGN };
		StmtInfo s2{ 2, STMT_ASSIGN };
		StmtInfo s3{ 3, STMT_PRINT };
		StmtInfo s4{ 4, STMT_ASSIGN };
		StmtInfo s5{ 5, STMT_CALL };
		StmtInfo s6{ 6, STMT_ASSIGN };
		StmtInfo s7{ 7, STMT_CALL };
		StmtInfo s8{ 8, STMT_ASSIGN };
		StmtInfo s9{ 9, STMT_ASSIGN };
		StmtInfo s10{ 10, STMT_CALL };
		StmtInfo s11{ 11, STMT_ASSIGN };
		StmtInfo s12{ 12, STMT_IF };
		StmtInfo s13{ 13, STMT_ASSIGN };
		StmtInfo s14{ 14, STMT_ASSIGN };
		StmtInfo s15{ 15, STMT_WHILE };
		StmtInfo s16{ 16, STMT_ASSIGN };

		std::vector<std::pair<StmtInfo, StmtInfo>> test_next_list = {
			{s1, s2}, {s2, s3}, {s4, s5}, {s5, s9}, {s6, s7}, {s7, s12}, {s9, s10}, {s10, s12},
			{s11, s6}, {s12, s13}, {s12, s15}, {s13, s14}, {s14, s8},
			{s14, s11}, {s15, s8}, {s15, s11}, {s15, s16}, {s16, s15}
		};
		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = test_next_list;

		std::vector<StmtInfo> stmt_list = { s1, s2, s3, s4, s5, s6, s7, s8, s9,
			s10, s11, s12, s13, s14, s15, s16
		};

		void TearDown() override {
			manager.reset();
			PKB::getInstance().resetCache();
		}
	};

	TEST_F(RelationPreprocessorManagerTest_NextBip, isPreviousBip_identifier) {
		std::vector<StmtInfo> true_list, false_list;
		std::set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		true_list.assign(set.begin(), set.end());

		std::sort(true_list.begin(), true_list.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list.begin(),
			true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& stmt : true_list) {
			EXPECT_TRUE(manager.isPreviousBip(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isPreviousBip(stmt.stmt_index));
		}
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, isNextBip_identifier) {
		std::vector<StmtInfo> true_list, false_list;
		std::set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		true_list.assign(set.begin(), set.end());

		std::sort(true_list.begin(), true_list.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list.begin(),
			true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& stmt : true_list) {
			EXPECT_TRUE(manager.isNextBip(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isNextBip(stmt.stmt_index));
		}
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, isNextBip_double_identifier) {
		std::vector<std::pair<StmtInfo, StmtInfo>> true_list = expected_pairs,
			false_list, all_list;
		std::set<StmtInfo> set;
		for (auto& s1 : stmt_list) {
			for (auto& s2 : stmt_list) {
				all_list.push_back({ s1, s2 });
			}
		}

		std::sort(true_list.begin(), true_list.end());
		std::sort(all_list.begin(), all_list.end());

		std::set_difference(all_list.begin(), all_list.end(), true_list.begin(),
			true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& pair : true_list) {
			stmt_index first_stmt_index = pair.first.stmt_index;
			stmt_index second_stmt_index = pair.second.stmt_index;

			EXPECT_TRUE(manager.isNextBip(first_stmt_index, second_stmt_index));
		}
		for (auto& pair : false_list) {
			stmt_index first_stmt_index = pair.first.stmt_index;
			stmt_index second_stmt_index = pair.second.stmt_index;

			EXPECT_FALSE(manager.isNextBip(first_stmt_index, second_stmt_index));
		}
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, getAllNextBipRelation) {
		auto v1 = manager.getAllNextBipRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, getPreviousBip_all) {
		auto v1 = manager.getPreviousBip();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, getNextBip_all) {
		auto v1 = manager.getNextBip();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(manager.getNextBipPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_NextBip, getNextBip_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getNextBip(stmt.stmt_index);
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

	TEST_F(RelationPreprocessorManagerTest_NextBip, getPreviousBip_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getPreviousBip(stmt.stmt_index);
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
}