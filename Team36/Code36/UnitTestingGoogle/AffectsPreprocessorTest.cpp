#include <iostream>

#include "pch.h"
#include "PKB/PKB.h"
#include "PKB/RelationPreprocessor/AffectsPreprocessor.h"

namespace UnitTesting {
	class AffectsPreprocessorTest : public testing::Test {
	protected:

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
		MonotypeRelationTable<StmtInfo> test_next_table = MonotypeRelationTable<StmtInfo>(test_next_list);

		std::vector<std::pair<StmtInfo, var_name>> test_usesS_list = {
			{s3, "i"}, {s4, "a"}, {s5, "x"}, {s5, "a"}, {s7, "a"}, {s8, "x"}, {s8, "y"}, {s9, "v"},
			{s10, "i"}, {s11, "x"}, {s12, "x"}, {s14, "z"}, {s14, "x"}, {s15, "z"}, {s16, "x"}, {s16, "y"},
			{s16, "z"}, {s19, "v"},
		};
		RelationTable<StmtInfo, var_name> test_usesS_table = RelationTable<StmtInfo, var_name>(test_usesS_list);

		std::vector<std::pair<StmtInfo, var_name>> test_modifiesS_list = {
			{s1, "x"}, {s2, "i"}, {s3, "x"}, {s3, "a"}, {s3, "v"}, {s4, "a"}, {s5, "a"}, {s6, "a"}, {s8, "x"},
			{s9, "x"}, {s9, "v"}, {s10, "i"}, {s11, "x"}, {s12, "x"}, {s13, "z"}, {s14, "z"}, {s15, "y"},
			{s16, "x"}, {s17, "x"}, {s18, "v"}
		};
		RelationTable<StmtInfo, var_name> test_modifiesS_table = RelationTable<StmtInfo, var_name>(test_modifiesS_list);

		std::vector<std::pair<proc_name, stmt_index>> test_procS_list = {
			{"First", 1}, {"First", 2},{"First", 3},{"First", 4},{"First", 5},
			{"First", 6}, {"First", 7},{"First", 8},{"First", 9},{"First", 10},
			{"First", 11}, {"First", 12},{"First", 13},{"First", 14},{"First", 15},
			{"First", 16}, {"Second", 17}, {"Second", 18}, {"Second", 19}
		};

		RelationTable<proc_name, stmt_index> test_procS_table = RelationTable<proc_name, stmt_index>(test_procS_list);

		AffectsPreprocessor processor = AffectsPreprocessor(test_next_table,
			test_usesS_table, test_modifiesS_table, test_procS_table, stmt_list);

		void TearDown() override {
			processor.reset();
		}
	};

	TEST_F(AffectsPreprocessorTest, evaluateWildAndWild) {
		EXPECT_EQ(processor.evaluateWildAndWild(), !expected_pairs.empty());
	}

	TEST_F(AffectsPreprocessorTest, evaluateConstantAndWild) {
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
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_UNKNOWN);
			EXPECT_TRUE(processor.evaluateConstantAndWild(stmt.stmt_index));
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_TRUE);
			EXPECT_TRUE(processor.evaluateConstantAndWild(stmt.stmt_index));
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_TRUE);
		}
		for (auto& stmt : false_list) {
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_UNKNOWN);
			EXPECT_FALSE(processor.evaluateConstantAndWild(stmt.stmt_index));
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_FALSE);
			EXPECT_FALSE(processor.evaluateConstantAndWild(stmt.stmt_index));
			EXPECT_EQ(processor.getAffecting()[stmt.stmt_index - 1], STATUS_FALSE);
		}
	}

	TEST_F(AffectsPreprocessorTest, evaluateWildAndConstant) {
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
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_UNKNOWN);
			EXPECT_TRUE(processor.evaluateWildAndConstant(stmt.stmt_index));
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_TRUE);
			EXPECT_TRUE(processor.evaluateWildAndConstant(stmt.stmt_index));
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_TRUE);
		}
		for (auto& stmt : false_list) {
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_UNKNOWN);
			EXPECT_FALSE(processor.evaluateWildAndConstant(stmt.stmt_index));
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_FALSE);
			EXPECT_FALSE(processor.evaluateWildAndConstant(stmt.stmt_index));
			EXPECT_EQ(processor.getAffected()[stmt.stmt_index - 1], STATUS_FALSE);
		}
	}

	TEST_F(AffectsPreprocessorTest, evaluateConstantAndConstant) {
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
			EXPECT_TRUE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
			EXPECT_EQ(processor.isCalculated(pair.first.stmt_index - 1, pair.second.stmt_index - 1), 1);
			EXPECT_TRUE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
		for (auto& pair : false_list) {
			EXPECT_FALSE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
			EXPECT_EQ(processor.isCalculated(pair.first.stmt_index - 1, pair.second.stmt_index - 1), 1);
			EXPECT_FALSE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
	}

	TEST_F(AffectsPreprocessorTest, evaluateSynonymAndSynonym) {
		auto v1 = processor.evaluateSynonymAndSynonym();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(AffectsPreprocessorTest, evaluateWildAndSynonym) {
		auto v1 = processor.evaluateWildAndSynonym();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.second);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(processor.isFullyPopulated());
	}

	TEST_F(AffectsPreprocessorTest, evaluateSynonymAndWild) {
		auto v1 = processor.evaluateSynonymAndWild();
		std::sort(v1.begin(), v1.end());

		std::unordered_set<StmtInfo> set;
		for (auto& pair : expected_pairs) {
			set.emplace(pair.first);
		}
		std::vector<StmtInfo> v2(set.size());
		std::copy(set.begin(), set.end(), v2.begin());
		std::sort(v2.begin(), v2.end());
		EXPECT_EQ(v1, v2);
		EXPECT_TRUE(processor.isFullyPopulated());
	}

	TEST_F(AffectsPreprocessorTest, evaluateConstantAndSynonym) {
		for (auto& stmt : stmt_list) {
			auto v1 = processor.evaluateConstantAndSynonym(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.first == stmt) {
					v2.push_back(pair.second);
				}
			}
			EXPECT_EQ(v1, v2);
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_TRUE(processor.isCalculated(stmt.stmt_index - 1, i));
				EXPECT_TRUE(processor.isDFSForwardComputed(stmt.stmt_index));
			}
		}
	}

	TEST_F(AffectsPreprocessorTest, evaluateSynonymAndConstant) {
		for (auto& stmt : stmt_list) {
			auto v1 = processor.evaluateSynonymAndConstant(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_TRUE(processor.isCalculated(i, stmt.stmt_index - 1));
				EXPECT_TRUE(processor.isDFSBackwardComputed(stmt.stmt_index));
			}
		}
	}

	TEST_F(AffectsPreprocessorTest, combinations) {
		// (s1, s2)

		auto v1 = processor.evaluateSynonymAndSynonym();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(processor.isFullyPopulated());

		// (s1, c)

		for (auto& stmt : stmt_list) {
			auto v1 = processor.evaluateSynonymAndConstant(stmt.stmt_index);
			std::sort(v1.begin(), v1.end());

			std::vector<StmtInfo> v2;
			for (auto& pair : expected_pairs) {
				if (pair.second == stmt) {
					v2.push_back(pair.first);
				}
			}
			EXPECT_EQ(v1, v2);
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_TRUE(processor.isCalculated(i, stmt.stmt_index - 1));
				EXPECT_TRUE(processor.isDFSBackwardComputed(stmt.stmt_index));
			}
		}

		// (c1, c2)
		std::vector < std::pair<StmtInfo, StmtInfo>> true_list2 = expected_pairs, false_list2, all_list2;
		for (auto& s1 : stmt_list) {
			for (auto& s2 : stmt_list) {
				all_list2.push_back({ s1, s2 });
			}
		}

		std::sort(true_list2.begin(), true_list2.end());
		std::sort(all_list2.begin(), all_list2.end());

		std::set_difference(all_list2.begin(), all_list2.end(), true_list2.begin(), true_list2.end(), std::inserter(false_list2, false_list2.begin()));

		for (auto& pair : true_list2) {
			EXPECT_TRUE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected true but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
		for (auto& pair : false_list2) {
			EXPECT_FALSE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}

		// (c, _)

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
			EXPECT_TRUE(processor.evaluateConstantAndWild(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(processor.evaluateConstantAndWild(stmt.stmt_index));
		}

		// (s1, _)
		auto v3 = processor.evaluateSynonymAndWild();
		std::sort(v3.begin(), v3.end());

		std::unordered_set<StmtInfo> set2;
		for (auto& pair : expected_pairs) {
			set2.emplace(pair.first);
		}
		std::vector<StmtInfo> v4(set.size());
		std::copy(set2.begin(), set2.end(), v4.begin());
		std::sort(v4.begin(), v4.end());
		EXPECT_EQ(v3, v4);
		EXPECT_TRUE(processor.isFullyPopulated());
	}

	TEST_F(AffectsPreprocessorTest, reset) {
		for (auto& stmt : stmt_list) {
			processor.evaluateSynonymAndConstant(stmt.stmt_index);
			for (int i = 0; i < stmt_list.size(); i++) {
				EXPECT_TRUE(processor.isCalculated(i, stmt.stmt_index - 1));
				EXPECT_TRUE(processor.isDFSBackwardComputed(stmt.stmt_index));
			}
		}
		for (auto& stmt : stmt_list) {
			processor.evaluateConstantAndSynonym(stmt.stmt_index);
			for (int i = 0; i < stmt_list.size(); i++) {
				EXPECT_TRUE(processor.isCalculated(stmt.stmt_index - 1, i));
				EXPECT_TRUE(processor.isDFSForwardComputed(stmt.stmt_index));
			}
		}
		processor.reset();
		for (int i = 0; i < stmt_list.size(); i++) {
			EXPECT_FALSE(processor.isDFSBackwardComputed(i + 1));
			EXPECT_FALSE(processor.isDFSForwardComputed(i + 1));
			for (int j = 0; j < stmt_list.size(); j++) {
				EXPECT_FALSE(processor.isCalculated(i, j));
			}
		}
		processor.evaluateSynonymAndSynonym();
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_FALSE(processor.isCacheEmpty());
		processor.reset();
		EXPECT_FALSE(processor.isFullyPopulated());
		EXPECT_TRUE(processor.isCacheEmpty());
	}
}