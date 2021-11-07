#include <iostream>

#include "pch.h"
#include "PKB.h"
#include "CFG.h"
#include "RelationPreprocessorManager.h"

namespace UnitTesting {
	class RelationPreprocessorManagerTest_AffectsBipT : public testing::Test {
	protected:
		RelationPreprocessorManagerTest_AffectsBipT() {
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
			cfg4_else_loop->addLine(16);

			cfg4_else->addLoop(cfg4_else_loop, 15);
			cfg4->addFork(cfg4_if, cfg4_else, 12);
			cfg2->addCall(cfg3, 5);
			cfg2->addCall(cfg4, 7);
			cfg3->addCall(cfg4, 10);

			for (auto& stmt : stmt_list) {
				PKB::getInstance().addStmt(stmt.stmt_type);
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
		CFG* cfg4_else_loop = new CFG();

		std::vector<CFG*> test_cfg_bips = { cfg1, cfg2, cfg3, cfg4, cfg4_if,
			cfg4_else, cfg4_else_loop
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

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = {
			{s4, s6}, {s4, s8}, {s4, s9}, {s4, s11}, {s4, s13}, {s4, s14}, {s4, s16},
			{s6, s8}, {s6, s16},
			{s9, s8}, {s9, s11}, {s9, s13}, {s9, s14}, {s9, s16},
			{s11, s8}, {s11, s13}, {s11, s14},
			{s13, s6}, {s13, s8}, {s13, s11}, {s13, s13}, {s13, s14}, {s13, s16},
			{s14, s8}, {s14, s11}, {s14, s13}, {s14, s14},
			{s16, s8}, {s16, s11}, {s16, s13}, {s16, s14}, {s16, s16}
		};

		std::vector<StmtInfo> stmt_list = { s1, s2, s3, s4, s5, s6, s7, s8, s9,
			s10, s11, s12, s13, s14, s15, s16
		};

		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> test_next_list = {
			{{1,{0}},{2,{0}}}, {{2,{0}},{3,{0}}}, {{4,{0}},{5,{0}}},
			{{5,{0}},{9,{0,5}}}, {{9,{0,5}},{10,{0,5}}},
			{{10,{0,5}},{12,{0,5,10}}}, {{12,{0,5,10}},{13,{0,5,10}}}, {{13,{0,5,10}},{14,{0,5,10}}},
			{{12,{0,5,10}},{15,{0,5,10}}}, {{15,{0,5,10}},{16,{0,5,10}}}, {{16,{0,5,10}},{15,{0,5,10}}},
			{{14,{0,5,10}},{11,{0,5}}}, {{15,{0,5,10}},{11,{0,5}}},
			{{11,{0,5}},{6,{0}}}, {{6,{0}},{7,{0}}},
			{{7,{0}},{12,{0,7}}}, {{12,{0,7}},{13,{0,7}}}, {{13,{0,7}},{14,{0,7}}}, {{12,{0,7}},{15,{0,7}}}, {{15,{0,7}},{16,{0,7}}}, {{16,{0,7}},{15,{0,7}}},
			{{14,{0,7}},{8,{0}}}, {{15,{0,7}},{8,{0}}}, {{9,{0}},{10,{0}}}, {{14,{0,10}},{11,{0}}}, {{15,{0,10}},{11,{0}}},
			{{12,{0}},{13,{0}}}, {{13,{0}},{14,{0}}}, {{12,{0}},{15,{0}}}, {{15,{0}},{16,{0}}}, {{16,{0}},{15,{0}}}
		};

		MonotypeRelationTable<LabelledProgLine> test_next_table = MonotypeRelationTable<LabelledProgLine>(test_next_list);

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		var_name v = "v";
		std::vector<var_name> var_list = { x, y, z, v };

		std::vector<std::pair<StmtInfo, var_name>> test_usesS_list = {
			{s3, v}, {s5, v}, {s5, x}, {s5, y}, {s5, z}, {s6, x}, {s7, v},
			{s7, x}, {s7, y}, {s7, z}, {s8, x}, {s8, y}, {s9,x},  {s10, v},
			{s10, x}, {s10, y}, {s10, z}, {s11, x}, {s11,y}, {s12, v}, {s12, x},
			{s12, y}, {s12, z}, {s13, x}, {s13, z}, {s14, x}, {s15, v}, {s15, x},
			{s15, y}, {s16, x}, {s16, y}
		};

		std::vector<std::pair<StmtInfo, var_name>> test_modifiesS_list = {
			{s1, x}, {s2, v}, {s4, x}, {s5, y}, {s5, z}, {s5, x}, {s6, y},
			{s7, x}, {s7, y}, {s8, z}, {s9, y}, {s10, x}, {s10, y}, {s11, z},
			{s12, x}, {s12, y}, {s13, x}, {s14, y}, {s15, y}, {s16, y}
		};

		proc_name proc_sally = "Sally";
		proc_name proc_bill = "Bill";
		proc_name proc_mary = "Mary";
		proc_name proc_john = "John";
		std::vector<proc_name> proc_list = { proc_sally, proc_bill, proc_mary, proc_john };

		std::vector<std::pair<proc_name, stmt_index>> test_procS_list = {
			{proc_sally, 1}, {proc_sally, 2}, {proc_sally, 3}, {proc_bill, 4}, {proc_bill, 5},
			{proc_bill, 6}, {proc_bill, 7}, {proc_bill, 8}, {proc_mary, 9}, {proc_mary, 10},
			{proc_mary, 11}, {proc_john, 12}, {proc_john, 13}, {proc_john, 14}, {proc_john, 15}
		};

		std::vector<LabelledProgLine> first_proglines = { {1, {0}}, { 4, {0 }} };

		void TearDown() override {
			manager.reset();
			PKB::getInstance().resetCache();
		}
	};

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, isAffectingBipT_identifier) {
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
			EXPECT_TRUE(manager.isAffectingBipT(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffectingBipT(stmt.stmt_index));
		}
	}

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, isAffectedBipT_identifier) {
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
			EXPECT_TRUE(manager.isAffectedBipT(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(manager.isAffectedBipT(stmt.stmt_index));
		}
	}

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, isAffectsBipT_double_identifier) {
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

			EXPECT_TRUE(manager.isAffectsBipT(first_stmt_index, second_stmt_index));
		}
		for (auto& pair : false_list) {
			stmt_index first_stmt_index = pair.first.stmt_index;
			stmt_index second_stmt_index = pair.second.stmt_index;

			EXPECT_FALSE(manager.isAffectsBipT(first_stmt_index, second_stmt_index));
		}
		EXPECT_TRUE(manager.getAffectsBipTPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, getAllAffectsBipTRelation) {
		auto v1 = manager.getAllAffectsBipTRelation();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(manager.getAffectsBipTPreprocessor().isFullyPopulated());
	}

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, getAffectingBipT_all) {
		auto v1 = manager.getAffectingBipT();
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

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, getAffectedBipT_all) {
		auto v1 = manager.getAffectedBipT();
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

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, getAffectedBipT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectedBipT(stmt.stmt_index);
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

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, getAffectingBipT_identifier) {
		for (auto& stmt : stmt_list) {
			auto v1 = manager.getAffectingBipT(stmt.stmt_index);
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

	TEST_F(RelationPreprocessorManagerTest_AffectsBipT, reset) {
		manager.getAllAffectsBipTRelation();
		EXPECT_TRUE(manager.getAffectsBipTPreprocessor().isFullyPopulated());
		EXPECT_FALSE(manager.getAffectsBipTPreprocessor().isCacheEmpty());
		manager.reset();
		EXPECT_FALSE(manager.getAffectsBipTPreprocessor().isFullyPopulated());
		EXPECT_TRUE(manager.getAffectsBipTPreprocessor().isCacheEmpty());
	}
}