#include <iostream>

#include "pch.h"
#include "PKB/PKB.h"
#include "PKB/RelationPreprocessor/NextBipTPreprocessor.h"

namespace UnitTesting {
	class NextBipTPreprocessorTest : public testing::Test {
	protected:

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

		std::vector<StmtInfo> stmt_list = { s1, s2, s3, s4, s5, s6, s7, s8, s9,
			s10, s11, s12, s13, s14, s15, s16
		};

		std::vector<std::pair<StmtInfo, StmtInfo>> test_next_list = {
			{s1, s2}, {s2, s3}, {s4, s5}, {s5, s9}, {s6, s7}, {s7, s12}, {s9, s10}, {s10, s12},
			{s11, s6}, {s12, s13}, {s12, s15}, {s13, s14}, {s14, s8},
			{s14, s11}, {s15, s8}, {s15, s11}, {s15, s16}, {s16, s15}
		};

		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> test_labelled_next_list = {
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

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = {
			{s1, s2}, {s1, s3}, {s2, s3},
			{s4, s5}, {s4, s6}, {s4, s7}, {s4, s8}, {s4, s9}, {s4, s10}, {s4, s11}, {s4, s12}, {s4, s13}, {s4, s14}, {s4, s15}, {s4, s16},
			{s5, s6}, {s5, s7}, {s5, s8}, {s5, s9}, {s5, s10}, {s5, s11}, {s5, s12}, {s5, s13}, {s5, s14}, {s5, s15}, {s5, s16},
			{s6, s7}, {s6, s8}, {s6, s12}, {s6, s13}, {s6, s14}, {s6, s15}, {s6, s16},
			{s7, s8}, {s7, s12}, {s7, s13}, {s7, s14}, {s7, s15}, {s7, s16},
			{s9, s6}, {s9, s7}, {s9, s8}, {s9, s10}, {s9, s11}, {s9, s12}, {s9, s13}, {s9, s14}, {s9, s15}, {s9, s16},
			{s10, s6}, {s10, s7}, {s10, s8}, {s10, s11}, {s10, s12}, {s10, s13}, {s10, s14}, {s10, s15}, {s10, s16},
			{s11, s6}, {s11, s7}, {s11, s8}, {s11, s12}, {s11, s13}, {s11, s14}, {s11, s15}, {s11, s16},
			{s12, s6}, {s12, s7}, {s12, s8}, {s12, s11}, {s12, s12}, {s12, s13}, {s12, s14}, {s12, s15}, {s12, s16},
			{s13, s6}, {s13, s7}, {s13, s8}, {s13, s11}, {s13, s12}, {s13, s13}, {s13, s14}, {s13, s15}, {s13, s16},
			{s14, s6}, {s14, s7}, {s14, s8}, {s14, s11}, {s14, s12}, {s14, s13}, {s14, s14}, {s14, s15}, {s14, s16},
			{s15, s6}, {s15, s7}, {s15, s8}, {s15, s11}, {s15, s12}, {s15, s13}, {s15, s14}, {s15, s15}, {s15, s16},
			{s16, s6}, {s16, s7}, {s16, s8}, {s16, s11}, {s16, s12}, {s16, s13}, {s16, s14}, {s16, s15}, {s16, s16}
		};

		MonotypeRelationTable<StmtInfo> test_next_table = MonotypeRelationTable<StmtInfo>(test_next_list);
		MonotypeRelationTable<LabelledProgLine> test_next_bip_table = MonotypeRelationTable<LabelledProgLine>(test_labelled_next_list);
		NextBipTPreprocessor processor = NextBipTPreprocessor(test_next_table, test_next_bip_table, stmt_list);

		void TearDown() override {
			processor.reset();
		}
	};

	TEST_F(NextBipTPreprocessorTest, evaluateWildAndWild) {
		EXPECT_EQ(processor.evaluateWildAndWild(), !expected_pairs.empty());
	}

	TEST_F(NextBipTPreprocessorTest, evaluateConstantAndWild) {
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
	}

	TEST_F(NextBipTPreprocessorTest, evaluateWildAndConstant) {
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
			EXPECT_TRUE(processor.evaluateWildAndConstant(stmt.stmt_index));
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(processor.evaluateWildAndConstant(stmt.stmt_index));
		}
	}

	TEST_F(NextBipTPreprocessorTest, evaluateConstantAndConstant) {
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
		}
		for (auto& pair : false_list) {
			EXPECT_FALSE(processor.evaluateConstantAndConstant(pair.first.stmt_index, pair.second.stmt_index)) << "Expected false but fail at " <<
				pair.first.stmt_index << " and" << pair.second.stmt_index;
		}
	}

	TEST_F(NextBipTPreprocessorTest, evaluateSynonymAndSynonym) {
		auto v1 = processor.evaluateSynonymAndSynonym();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(NextBipTPreprocessorTest, evaluateWildAndSynonym) {
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

	TEST_F(NextBipTPreprocessorTest, evaluateSynonymAndWild) {
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

	TEST_F(NextBipTPreprocessorTest, evaluateConstantAndSynonym) {
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
		}
	}

	TEST_F(NextBipTPreprocessorTest, evaluateSynonymAndConstant) {
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
		}
	}

	TEST_F(NextBipTPreprocessorTest, combinations) {
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

	TEST_F(NextBipTPreprocessorTest, reset) {
		processor.evaluateSynonymAndSynonym();
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_FALSE(processor.isCacheEmpty());
		processor.reset();
		EXPECT_FALSE(processor.isFullyPopulated());
		EXPECT_TRUE(processor.isCacheEmpty());
	}
}