#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "CFGRelationsManager.h"

namespace UnitTesting {
	class CFGRelationsManagerTest_AffectsT : public testing::Test {
	protected:
		CFGRelationsManagerTest_AffectsT() {
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

			manager.update();
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
			{s1, s8}, {s1, s12}, {s1, s14}, {s1, s15}, {s1, s16}, {s2, s10}, {s4, s4 },
			{s10, s10}, {s12, s14}, {s12, s15}, {s12, s16}, {s13, s14}, {s13, s15},
			{s13, s16}, {s14, s15}, {s14, s16}, {s15, s16}
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

	TEST_F(CFGRelationsManagerTest_AffectsT, isAffectsTEmpty) {
		EXPECT_FALSE(manager.isAffectsTEmpty());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isNonEmpty());
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, isAffectingT_identifier) {
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
			EXPECT_TRUE(manager.isAffectingT(stmt.stmt_index));
			EXPECT_TRUE(manager.isAffectingT(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffectingT(stmt.stmt_index));
			EXPECT_FALSE(manager.isAffectingT(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, isAffectedT_identifier) {
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
			EXPECT_TRUE(manager.isAffectedT(stmt.stmt_index));
			EXPECT_TRUE(manager.isAffectedT(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffectedT(stmt.stmt_index));
			EXPECT_FALSE(manager.isAffectedT(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, isAffectsT_double_identifier) {
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
			EXPECT_TRUE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;

			EXPECT_EQ(manager.getAffectsTPreprocessor().isCalculated(pair.first.stmt_index - 1, pair.second.stmt_index - 1), 1);
			EXPECT_TRUE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
		for (auto& pair : false_list) {
			EXPECT_FALSE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
			EXPECT_FALSE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, getAllAffectsTRelation) {
		auto v1 = manager.getAllAffectsTRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, getAffectingT_all) {
		auto v1 = manager.getAffectingT();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, getAffectedT_all) {
		auto v1 = manager.getAffectedT();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, getAffectingT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectingT(stmt.stmt_index);
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

	TEST_F(CFGRelationsManagerTest_AffectsT, getAffectedT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectedT(stmt.stmt_index);
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

	TEST_F(CFGRelationsManagerTest_AffectsT, combinations) {
		// _, _
		EXPECT_FALSE(manager.isAffectsTEmpty());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isNonEmpty());

		// c, _
		std::vector<StmtInfo> true_list_c_wild, false_list_c_wild;
		std::set<StmtInfo> set_c_wild;
		for (auto& pair : expected_pairs) {
			set_c_wild.emplace(pair.first);
		}
		true_list_c_wild.assign(set_c_wild.begin(), set_c_wild.end());

		std::sort(true_list_c_wild.begin(), true_list_c_wild.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list_c_wild.begin(), true_list_c_wild.end(), std::inserter(false_list_c_wild, false_list_c_wild.begin()));

		for (auto& stmt : true_list_c_wild) {
			EXPECT_TRUE(manager.isAffectingT(stmt.stmt_index));
			EXPECT_TRUE(manager.isAffectingT(stmt.stmt_index));
		}
		for (auto& stmt : false_list_c_wild) {
			EXPECT_FALSE(manager.isAffectingT(stmt.stmt_index));
			EXPECT_FALSE(manager.isAffectingT(stmt.stmt_index));
		}

		// _, c
		std::vector<StmtInfo> true_list_wild_c, false_list_wild_c;
		std::set<StmtInfo> set_wild_c;
		for (auto& pair : expected_pairs) {
			set_wild_c.emplace(pair.second);
		}
		true_list_wild_c.assign(set_wild_c.begin(), set_wild_c.end());

		std::sort(true_list_wild_c.begin(), true_list_wild_c.end());
		std::sort(stmt_list.begin(), stmt_list.end());

		std::set_difference(stmt_list.begin(), stmt_list.end(), true_list_wild_c.begin(),
			true_list_wild_c.end(), std::inserter(false_list_wild_c, false_list_wild_c.begin()));

		for (auto& stmt : true_list_wild_c) {
			EXPECT_TRUE(manager.isAffectedT(stmt.stmt_index));
			EXPECT_TRUE(manager.isAffectedT(stmt.stmt_index));
		}
		for (auto& stmt : false_list_wild_c) {
			EXPECT_FALSE(manager.isAffectedT(stmt.stmt_index));
			EXPECT_FALSE(manager.isAffectedT(stmt.stmt_index));
		}

		// c, c
		std::vector < std::pair<StmtInfo, StmtInfo>> true_list_c_c = expected_pairs, false_list_c_c, all_list_c_c;
		std::set<StmtInfo> set_c_c;
		for (auto& s1 : stmt_list) {
			for (auto& s2 : stmt_list) {
				all_list_c_c.push_back({ s1, s2 });
			}
		}

		std::sort(true_list_c_c.begin(), true_list_c_c.end());
		std::sort(all_list_c_c.begin(), all_list_c_c.end());

		std::set_difference(all_list_c_c.begin(), all_list_c_c.end(), true_list_c_c.begin(), true_list_c_c.end(), std::inserter(false_list_c_c, false_list_c_c.begin()));

		for (auto& pair : true_list_c_c) {
			EXPECT_TRUE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
			EXPECT_EQ(manager.getAffectsTPreprocessor().isCalculated(pair.first.stmt_index - 1, pair.second.stmt_index - 1), 1);
			EXPECT_TRUE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
		for (auto& pair : false_list_c_c) {
			EXPECT_FALSE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
			EXPECT_FALSE(manager.isAffectsT(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}

		// s, _
		auto v1_s_wild = manager.getAffectingT();
		std::sort(v1_s_wild.begin(), v1_s_wild.end());

		std::unordered_set<StmtInfo> set_s_wild;
		for (auto& pair : expected_pairs) {
			set_s_wild.emplace(pair.first);
		}
		std::vector<StmtInfo> v2_s_wild(set_s_wild.size());
		std::copy(set_s_wild.begin(), set_s_wild.end(), v2_s_wild.begin());
		std::sort(v2_s_wild.begin(), v2_s_wild.end());
		EXPECT_EQ(v1_s_wild, v2_s_wild);
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());

		// _, s
		auto v1_wild_s = manager.getAffectedT();
		std::sort(v1_wild_s.begin(), v1_wild_s.end());

		std::unordered_set<StmtInfo> set_wild_s;
		for (auto& pair : expected_pairs) {
			set_wild_s.emplace(pair.second);
		}
		std::vector<StmtInfo> v2_wild_s(set_wild_s.size());
		std::copy(set_wild_s.begin(), set_wild_s.end(), v2_wild_s.begin());
		std::sort(v2_wild_s.begin(), v2_wild_s.end());
		EXPECT_EQ(v1_wild_s, v2_wild_s);
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());

		// s, c
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectingT(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
		}

		// c, s
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectedT(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.first == stmt) {
					v2.push_back(pair.second);
				}
			}
			EXPECT_EQ(v1, v2);
		}

		// s1, s2
		auto v1_s_s = manager.getAllAffectsTRelation();
		std::sort(v1_s_s.begin(), v1_s_s.end());
		EXPECT_EQ(v1_s_s, expected_pairs);
		EXPECT_TRUE(manager.getAffectsPreprocessor().isFullyPopulated());
		EXPECT_EQ(v1_s_s, expected_pairs);
	}

	TEST_F(CFGRelationsManagerTest_AffectsT, reset) {
		manager.getAllAffectsTRelation();
		EXPECT_TRUE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isCacheEmpty());
		manager.reset();
		EXPECT_FALSE(manager.getAffectsTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsTPreprocessor().isCacheEmpty());
	}
}