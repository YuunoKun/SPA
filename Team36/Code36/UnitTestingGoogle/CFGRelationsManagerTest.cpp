#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "CFGRelationsManager.h"

namespace UnitTesting {
	class CFGRelationsManagerTest_NextT : public testing::Test {
	protected:
		CFGRelationsManagerTest_NextT() {
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

			manager = new CFGRelationsManager();
		}

		CFGRelationsManager* manager;

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

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
			manager->reset();
		}
	};

	TEST_F(CFGRelationsManagerTest_NextT, isNextTEmpty) {
		EXPECT_FALSE(manager->isNextTEmpty());
	}

	TEST_F(CFGRelationsManagerTest_NextT, isPreviousT) {
		EXPECT_TRUE(manager->isPreviousT(1));
		EXPECT_TRUE(manager->isPreviousT(2));
		EXPECT_TRUE(manager->isPreviousT(3));
		EXPECT_TRUE(manager->isPreviousT(4));
		EXPECT_TRUE(manager->isPreviousT(5));
		EXPECT_FALSE(manager->isPreviousT(6));
	}

	TEST_F(CFGRelationsManagerTest_NextT, isNextT_identifier) {
		EXPECT_FALSE(manager->isNextT(1));
		EXPECT_TRUE(manager->isNextT(2));
		EXPECT_TRUE(manager->isNextT(3));
		EXPECT_TRUE(manager->isNextT(4));
		EXPECT_TRUE(manager->isNextT(5));
		EXPECT_TRUE(manager->isNextT(6));
	}

	TEST_F(CFGRelationsManagerTest_NextT, isNextT_all) {
		EXPECT_FALSE(manager->isNextT(1, 1));
		EXPECT_TRUE(manager->isNextT(1, 2));
		EXPECT_TRUE(manager->isNextT(1, 3));
		EXPECT_TRUE(manager->isNextT(1, 4));
		EXPECT_TRUE(manager->isNextT(1, 5));
		EXPECT_TRUE(manager->isNextT(1, 6));

		EXPECT_FALSE(manager->isNextT(2, 1));
		EXPECT_TRUE(manager->isNextT(2, 2));
		EXPECT_TRUE(manager->isNextT(2, 3));
		EXPECT_TRUE(manager->isNextT(2, 4));
		EXPECT_TRUE(manager->isNextT(2, 5));
		EXPECT_TRUE(manager->isNextT(2, 6));

		EXPECT_FALSE(manager->isNextT(3, 1));
		EXPECT_TRUE(manager->isNextT(3, 2));
		EXPECT_TRUE(manager->isNextT(3, 3));
		EXPECT_TRUE(manager->isNextT(3, 4));
		EXPECT_TRUE(manager->isNextT(3, 5));
		EXPECT_TRUE(manager->isNextT(3, 6));

		EXPECT_FALSE(manager->isNextT(4, 1));
		EXPECT_TRUE(manager->isNextT(4, 2));
		EXPECT_TRUE(manager->isNextT(4, 3));
		EXPECT_TRUE(manager->isNextT(4, 4));
		EXPECT_TRUE(manager->isNextT(4, 5));
		EXPECT_TRUE(manager->isNextT(4, 6));

		EXPECT_FALSE(manager->isNextT(5, 1));
		EXPECT_TRUE(manager->isNextT(5, 2));
		EXPECT_TRUE(manager->isNextT(5, 3));
		EXPECT_TRUE(manager->isNextT(5, 4));
		EXPECT_TRUE(manager->isNextT(5, 5));
		EXPECT_TRUE(manager->isNextT(5, 6));

		EXPECT_FALSE(manager->isNextT(6, 1));
		EXPECT_FALSE(manager->isNextT(6, 2));
		EXPECT_FALSE(manager->isNextT(6, 3));
		EXPECT_FALSE(manager->isNextT(6, 4));
		EXPECT_FALSE(manager->isNextT(6, 5));
		EXPECT_FALSE(manager->isNextT(6, 6));

		EXPECT_FALSE(manager->isNextT(1, 1));
		EXPECT_TRUE(manager->isNextT(1, 2));
		EXPECT_TRUE(manager->isNextT(1, 3));
		EXPECT_TRUE(manager->isNextT(1, 4));
		EXPECT_TRUE(manager->isNextT(1, 5));
		EXPECT_TRUE(manager->isNextT(1, 6));
	}
	TEST_F(CFGRelationsManagerTest_NextT, getAllNextTRelation) {
		auto v1 = manager->getAllNextTRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(CFGRelationsManagerTest_NextT, getNextT_double_identifier) {
		auto v1 = manager->getNextT();
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

	TEST_F(CFGRelationsManagerTest_NextT, getPreviousT_all) {
		auto v1 = manager->getPreviousT();
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

	TEST_F(CFGRelationsManagerTest_NextT, getNextT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager->getNextT(stmt.stmt_index);
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

	TEST_F(CFGRelationsManagerTest_NextT, getPreviousT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager->getPreviousT(stmt.stmt_index);
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

	TEST_F(CFGRelationsManagerTest_NextT, reset) {
		manager->getAllNextTRelation();
		EXPECT_TRUE(manager->getNextTProcessor().isFullyPopulated());
		EXPECT_FALSE(manager->getNextTProcessor().isCacheEmpty());
		manager->reset();
		EXPECT_FALSE(manager->getNextTProcessor().isFullyPopulated());
		EXPECT_TRUE(manager->getNextTProcessor().isCacheEmpty());
	}

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

			manager = new CFGRelationsManager();
		}

		CFGRelationsManager* manager;

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

		std::vector<proc_name> proc_list = { "First", "Second" };
		std::vector<var_name> var_list = { "x", "i", "a", "y", "z", "v" };

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
			{s3, "i"}, {s4, "a"}, {s5, "x"}, {s5, "a"}, {s7, "a"}, {s8, "x"}, {s8, "y"}, {s9, "v"},
			{s10, "i"}, {s11, "x"}, {s12, "x"}, {s14, "z"}, {s14, "x"}, {s15, "z"}, {s16, "x"}, {s16, "y"},
			{s16, "z"}, {s19, "v"},
		};

		std::vector<std::pair<StmtInfo, var_name>> test_modifiesS_list = {
			{s1, "x"}, {s2, "i"}, {s3, "x"}, {s3, "a"}, {s3, "v"}, {s4, "a"}, {s5, "a"}, {s6, "a"}, {s8, "x"},
			{s9, "x"}, {s9, "v"}, {s10, "i"}, {s11, "x"}, {s12, "x"}, {s13, "z"}, {s14, "z"}, {s15, "y"},
			{s16, "x"}, {s17, "x"}, {s18, "v"}
		};

		std::vector<std::pair<proc_name, stmt_index>> test_procS_list = {
			{"First", 1}, {"First", 2},{"First", 3},{"First", 4},{"First", 5},
			{"First", 6}, {"First", 7},{"First", 8},{"First", 9},{"First", 10},
			{"First", 11}, {"First", 12},{"First", 13},{"First", 14},{"First", 15},
			{"First", 16}, {"Second", 17}, {"Second", 18}, {"Second", 19}
		};

		// Sample program:
		//procedure First{
		//01        x = 0;
		//02        i = 5;
		//03        while (i != 0) {
		//04            a = a + 1;
		//05            if (x < 3) then {
		//06                read a;
		//              } else {
		//07                print a;
		//              }
		//08            x = x + 2 * y;
		//09            call Second;
		//10            i = i - 1;
		//          }
		//11        if (x == 1) then {
		//12            x = x + 1; }
		//	  	    else {
		//13            z = 1;
		//          }
		//14        z = z + x + i;
		//15        y = z + 2;
		//16        x = x * y + z;
		//}
		//procedure Second{
		//17	x = 5;
		//18	v = z;
		//19	print v;
		//}
		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
			manager->reset();
		}
	};

	TEST_F(CFGRelationsManagerTest_Affects, isAffectsEmpty) {
		EXPECT_FALSE(manager->isAffectsEmpty());
		EXPECT_TRUE(manager->getAffectsProcessor().isNonEmpty());
	}

	TEST_F(CFGRelationsManagerTest_Affects, isAffector_identifier) {
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
			EXPECT_TRUE(manager->isAffector(stmt.stmt_index)) << "Expected true but fail at " << stmt.stmt_index;
			EXPECT_TRUE(manager->getAffectsProcessor().getAffecting()[stmt.stmt_index - 1]);
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager->isAffector(stmt.stmt_index)) << "Expected false but fail at " << stmt.stmt_index;
			EXPECT_FALSE(manager->getAffectsProcessor().getAffecting()[stmt.stmt_index - 1]);
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
			EXPECT_TRUE(manager->isAffected(stmt.stmt_index)) << "Expected true but fail at " << stmt.stmt_index;
			EXPECT_TRUE(manager->getAffectsProcessor().getAffected()[stmt.stmt_index - 1]);
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager->isAffected(stmt.stmt_index)) << "Expected false but fail at " << stmt.stmt_index;
			EXPECT_FALSE(manager->getAffectsProcessor().getAffected()[stmt.stmt_index - 1]) << "Expected false but fail at " << stmt.stmt_index;;
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
			stmt_index first_stmt_index = pair.first.stmt_index;
			stmt_index second_stmt_index = pair.second.stmt_index;

			EXPECT_TRUE(manager->isAffects(first_stmt_index, second_stmt_index)) << "Expected true but fail at " <<
				first_stmt_index << " and" << second_stmt_index;
		}
		for (auto& pair : false_list) {
			stmt_index first_stmt_index = pair.first.stmt_index;
			stmt_index second_stmt_index = pair.second.stmt_index;

			EXPECT_FALSE(manager->isAffects(first_stmt_index, second_stmt_index)) << "Expected false but fail at " <<
				first_stmt_index << " and" << second_stmt_index;
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAllAffectsRelation) {
		auto v1 = manager->getAllAffectsRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(manager->getAffectsProcessor().isFullyPopulated());
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffecting_all) {
		auto v1 = manager->getAffector();
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

	TEST_F(CFGRelationsManagerTest_Affects, getAffected_all) {
		auto v1 = manager->getAffected();
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

	TEST_F(CFGRelationsManagerTest_Affects, getAffected_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager->getAffected(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.first == stmt) {
					v2.push_back(pair.second);
				}
			}
			EXPECT_EQ(v1, v2);
			EXPECT_TRUE(manager->getAffectsProcessor().isDFSForwardComputed(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, getAffecting_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager->getAffector(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
			EXPECT_TRUE(manager->getAffectsProcessor().isDFSBackwardComputed(stmt.stmt_index));
		}
	}

	TEST_F(CFGRelationsManagerTest_Affects, reset) {
		manager->getAllAffectsRelation();
		EXPECT_TRUE(manager->getAffectsProcessor().isFullyPopulated());
		EXPECT_FALSE(manager->getAffectsProcessor().isCacheEmpty());
		manager->reset();
		EXPECT_FALSE(manager->getAffectsProcessor().isFullyPopulated());
		EXPECT_TRUE(manager->getAffectsProcessor().isCacheEmpty());
	}
}