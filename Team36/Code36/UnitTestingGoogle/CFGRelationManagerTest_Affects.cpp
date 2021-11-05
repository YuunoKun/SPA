#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "CFGRelationsManager.h"

namespace UnitTesting {
	class CFGRelationsManagerTest_Affects : public testing::Test {
	protected:
		CFGRelationsManagerTest_Affects() {
			PKB::getInstance().resetCache();

			for (auto& stmt : stmt_list) {
				PKB::getInstance().addStmt(stmt.stmt_type);
			}

			for (auto& pair : test_next_list) {
				PKB::getInstance().addNext(pair.first.stmt_index, pair.second.stmt_index);
			}

			for (auto& proc : proc_list) {
				PKB::getInstance().addProcedure(proc);
			}

			for (auto& var : var_list) {
				PKB::getInstance().addVariable(var);
			}

			for (auto& pair : test_usesS_list) {
				PKB::getInstance().addUsesS(pair.first.stmt_index, pair.second);
			}

			for (auto& pair : test_modifiesS_list) {
				PKB::getInstance().addModifiesS(pair.first.stmt_index, pair.second);
			}

			for (auto& pair : test_procS_list) {
				PKB::getInstance().addProcContains(pair.first, pair.second);
			}

			manager.reset();
		}

		CFGRelationsManager manager;

		StmtInfo s1{ 1, STMT_ASSIGN };
		StmtInfo s2{ 2, STMT_ASSIGN };
		StmtInfo s3{ 3, STMT_WHILE };
		StmtInfo s4{ 4, STMT_ASSIGN };
		StmtInfo s5{ 5, STMT_IF };
		StmtInfo s6{ 6, STMT_READ };
		StmtInfo s7{ 7, STMT_PRINT };
		StmtInfo s8{ 8, STMT_ASSIGN };
		StmtInfo s9{ 9, STMT_CALL };
		StmtInfo s10{ 10, STMT_ASSIGN };
		StmtInfo s11{ 11, STMT_IF };
		StmtInfo s12{ 12, STMT_ASSIGN };
		StmtInfo s13{ 13, STMT_ASSIGN };
		StmtInfo s14{ 14, STMT_ASSIGN };
		StmtInfo s15{ 15, STMT_ASSIGN };
		StmtInfo s16{ 16, STMT_ASSIGN };
		StmtInfo s17{ 17, STMT_ASSIGN };
		StmtInfo s18{ 18, STMT_ASSIGN };
		StmtInfo s19{ 19, STMT_PRINT };

		std::vector<StmtInfo> stmt_list = { s1, s2, s3, s4, s5, s6, s7, s8, s9,
			s10, s11, s12, s13, s14, s15, s16, s17, s18, s19
		};

		proc_name p1 = "p1";
		proc_name p2 = "p2";

		var_name a = "a";
		var_name i = "i";
		var_name v = "v";
		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		std::vector<proc_name> proc_list = { p1, p2 };
		std::vector<var_name> var_list = { a, i, v, x, y, z };

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = {
			{s1, s8}, {s1, s12}, {s1, s14}, {s1, s16}, {s2, s10}, {s4, s4 },
			{s10, s10}, {s12, s14}, {s12, s16}, {s13, s14}, {s14, s15},
			{s14, s16}, {s15, s16}
		};

		std::vector<std::pair<StmtInfo, StmtInfo>> test_next_list = {
			{s1, s2}, {s2, s3}, {s3, s4}, {s4, s5}, {s5, s6}, {s5, s7}, {s6, s8},
			{s7, s8}, {s8, s9}, {s9, s10}, {s10, s3}, {s3, s11}, {s11, s12},
			{s11, s13}, {s12, s14}, {s13, s14}, {s14, s15}, {s15, s16}, {s17, s18},
			{s18, s19}
		};

		std::vector<std::pair<StmtInfo, var_name>> test_usesS_list = {
			{s3, i}, {s4, a}, {s5, x}, {s5, a}, {s7, a}, {s8, x}, {s8, y}, {s9, v},
			{s10, i}, {s11, x}, {s12, x}, {s14, z}, {s14, x}, {s15, z}, {s16, x}, {s16, y},
			{s16, z}, {s19, v},
		};

		std::vector<std::pair<StmtInfo, var_name>> test_modifiesS_list = {
			{s1, x}, {s2, i}, {s3, x}, {s3, a}, {s3, v}, {s4, a}, {s5, a}, {s6, a}, {s8, x},
			{s9, x}, {s9, v}, {s10, i}, {s11, x}, {s12, x}, {s13, z}, {s14, z}, {s15, y},
			{s16, x}, {s17, x}, {s18, v}
		};

		std::vector<std::pair<proc_name, stmt_index>> test_procS_list = {
			{p1, 1}, {p1, 2}, {p1, 3}, {p1, 4}, {p1, 5},
			{p1, 6}, {p1, 7}, {p1, 8}, {p1, 9}, {p1, 10},
			{p1, 11}, {p1, 12}, {p1, 13}, {p1, 14}, {p1, 15},
			{p1, 16}, {p2, 17}, {p2, 18}, {p2, 19}
		};

		void TearDown() override {
			manager.reset();
			PKB::getInstance().resetCache();
		}
	};

	TEST_F(CFGRelationsManagerTest_Affects, isAffectsEmpty) {
		EXPECT_FALSE(manager.isAffectsEmpty());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isNonEmpty());
	}

	TEST_F(CFGRelationsManagerTest_Affects, isAffecting_identifier) {
		std::vector<StmtInfo> true_list, false_list;
		std::set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		true_list.assign(set.begin(), set.end());

		std::sort(true_list.begin(), true_list.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list.begin(), true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& stmt : true_list) {
			EXPECT_TRUE(manager.isAffecting(stmt.stmt_index));
			EXPECT_TRUE(manager.getAffectsPreprocessor().getAffecting()[stmt.stmt_index - 1]);
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffecting(stmt.stmt_index));
			EXPECT_FALSE(manager.getAffectsPreprocessor().getAffecting()[stmt.stmt_index - 1]);
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, isAffected_identifier) {
		std::vector<StmtInfo> true_list, false_list;
		std::set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		true_list.assign(set.begin(), set.end());

		std::sort(true_list.begin(), true_list.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list.begin(), true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& stmt : true_list) {
			EXPECT_TRUE(manager.isAffected(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffected(stmt.stmt_index));
			EXPECT_FALSE(manager.getAffectsPreprocessor().getAffected()[stmt.stmt_index - 1]);
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, isAffects_double_identifier) {
		std::vector < std::pair<StmtInfo, StmtInfo>> true_list = expected_pairs, false_list, all_list;
		std::set<StmtInfo> set;
		for (auto& s1 : stmt_list) {
			for (auto& s2 : stmt_list) {
				all_list.push_back({ s1, s2 });
			}
		}

		std::sort(true_list.begin(), true_list.end());
		std::sort(all_list.begin(), all_list.end());

		std::set_difference(all_list.begin(), all_list.end(), true_list.begin(), true_list.end(), std::inserter(false_list, false_list.begin()));

		for (auto& pair : true_list) {
			stmt_index p1_stmt_index = pair.first.stmt_index;
			stmt_index p2_stmt_index = pair.second.stmt_index;

			EXPECT_TRUE(manager.isAffects(p1_stmt_index, p2_stmt_index));
		}
		for (auto& pair : false_list) {
			stmt_index p1_stmt_index = pair.first.stmt_index;
			stmt_index p2_stmt_index = pair.second.stmt_index;

			EXPECT_FALSE(manager.isAffects(p1_stmt_index, p2_stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAllAffectsRelation) {
		auto v1 = manager.getAllAffectsRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffecting_all) {
		auto v1 = manager.getAffecting();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffected_all) {
		auto v1 = manager.getAffected();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffected_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffected(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.first == stmt) {
					v2.push_back(pair.second);
				}
			}
			EXPECT_EQ(v1, v2);
			EXPECT_TRUE(manager.getAffectsPreprocessor().isDFSForwardComputed(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffecting_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffecting(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
			EXPECT_TRUE(manager.getAffectsPreprocessor().isDFSBackwardComputed(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, reset) {
		manager.getAllAffectsRelation();
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
		EXPECT_FALSE(manager.getAffectsPreprocessor().isCacheEmpty());
		manager.reset();
		EXPECT_FALSE(manager.getAffectsPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isCacheEmpty());
	}
}