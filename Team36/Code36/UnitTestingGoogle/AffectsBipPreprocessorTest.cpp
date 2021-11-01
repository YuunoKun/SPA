#include <iostream>

#include "pch.h"
#include "PKB.h"
#include <AffectsBipPreprocessor.h>

namespace UnitTesting {
	class AffectsBipPreprocessorTest : public testing::Test {
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

		//procedure Sally{
		//01	x = 5;
		//02	v = z;
		//03	print v;
		//}
		//	procedure Bill{
		//04      x = 5;
		//05      call Mary;
		//06      y = x + 6;
		//07      call John;
		//08      z = x * y + 2; }

		//procedure Mary{
		//09      y = x * 3;
		//10      call John;
		//11      z = x + y; }

		//procedure John{
		//12      if (i > 0) then {
		//13              x = x + z;
		//14              y = x;
		// } }
		//		  else {
		//15          while (v > 5) {
		//16               y = x * y;
		//			  }
		//         }
		//	}

		std::vector<std::pair<StmtInfo, StmtInfo>> expected_pairs = {
			{s4, s6}, {s4, s8}, {s4, s9}, {s4, s11}, {s4, s13}, {s4, s16},
			{s6, s8}, {s6, s16},
			{s9, s11}, {s9, s16}, {s11, s13},
			{s13, s6}, {s13, s8}, {s13, s11}, {s13, s13}, {s13, s14}, {s13, s16},
			{s14, s8}, {s14, s11},
			{s16, s8}, {s16, s11}, {s16, s16}
		};

		std::vector<std::pair<LabelledProgLine, LabelledProgLine>> test_next_list = {
			{{1,0}, {2,0}}, {{2,0}, {3,0}}, {{4,0}, {5,0}}, {{5,0}, {9,5}},
			{{9,5}, {10,5}}, {{10,5}, {12,10}}, {{12,10},{13,10}}, {{13, 10}, {14, 10}},
			{{12,10},{15,10}},
			{{15,10},{16,10}}, {{16,10},{15,10}}, {{14, 10}, {11, 5}}, {{15, 10}, {11, 5}}, {{11,5}, {6,0}},
			{{6,0}, {7,0}}, {{7,0}, {12,7}}, {{12,7},{13,7}}, {{13,7}, {14,7}}, {{12,7},{15,7}},
			{{15,7},{16,7}}, {{16,7},{15,7}}, {{14, 7}, {8,0}}, {{15, 7}, {8,0}}
		};

		MonotypeRelationTable<LabelledProgLine> test_next_table = MonotypeRelationTable<LabelledProgLine>(test_next_list);

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";
		var_name v = "v";

		std::vector<std::pair<StmtInfo, var_name>> test_usesS_list = {
			{s3, v}, {s5, v}, {s5, x}, {s5, y}, {s5, z}, {s6, x}, {s7, v},
			{s7, x}, {s7, y}, {s7, z}, {s8, x}, {s8, y}, {s9,x},  {s10, v},
			{s10, x}, {s10, y}, {s10, z}, {s11, x}, {s11,y}, {s12, v}, {s12, x},
			{s12, y}, {s12, z}, {s13, x}, {s13, z}, {s14, x}, {s15, v}, {s15, x},
			{s15, y}, {s16, x}, {s16, y}
		};
		RelationTable<StmtInfo, var_name> test_usesS_table = RelationTable<StmtInfo, var_name>(test_usesS_list);

		std::vector<std::pair<StmtInfo, var_name>> test_modifiesS_list = {
			{s1, x}, {s2, v}, {s4, x}, {s5, y}, {s5, z}, {s5, x}, {s6, y},
			{s7, x}, {s7, y}, {s8, z}, {s9, y}, {s10, x}, {s10, y}, {s11, z},
			{s12, x}, {s12, y}, {s13, x}, {s14, y}, {s15, y}, {s16, y}
		};
		RelationTable<StmtInfo, var_name> test_modifiesS_table = RelationTable<StmtInfo, var_name>(test_modifiesS_list);

		proc_name sally = "Sally";
		proc_name bill = "Bill";
		proc_name mary = "Mary";
		proc_name john = "John";

		std::vector<std::pair<proc_name, stmt_index>> test_procS_list = {
			{sally, 1}, {sally, 2}, {sally, 3}, {bill, 4}, {bill, 5},
			{bill, 6}, {bill, 7}, {bill, 8}, {mary, 9}, {mary, 10},
			{mary, 11}, {john, 12}, {john, 13}, {john, 14}, {john, 15}
		};

		RelationTable<proc_name, stmt_index> test_procS_table = RelationTable<proc_name, stmt_index>(test_procS_list);

		std::vector<LabelledProgLine> first_proglines = { {1, 0}, { 4, 0 } };

		AffectsBipPreprocessor processor = AffectsBipPreprocessor(test_next_table,
			test_usesS_table, test_modifiesS_table, test_procS_table, first_proglines, stmt_list);

		virtual void SetUp() override {
			// Code here will be called immediately after the constructor (right
			// before each test).
		}

		void TearDown() override {
			// Code here will be called immediately after each test (right
			// before the destructor).
			processor.reset();
		}
	};

	TEST_F(AffectsBipPreprocessorTest, evaluateWildAndWild) {
		EXPECT_EQ(processor.evaluateWildAndWild(), !expected_pairs.empty());
	}

	TEST_F(AffectsBipPreprocessorTest, evaluateConstantAndWild) {
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
			EXPECT_TRUE(processor.evaluateConstantAndWild(stmt.stmt_index)) << "Expected true but fail at " << stmt.stmt_index;
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(processor.evaluateConstantAndWild(stmt.stmt_index)) << "Expected false but fail at " << stmt.stmt_index;
		}
	}

	TEST_F(AffectsBipPreprocessorTest, evaluateWildAndConstant) {
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
			EXPECT_TRUE(processor.evaluateWildAndConstant(stmt.stmt_index)) << "Expected true but fail at " << stmt.stmt_index;
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(processor.evaluateWildAndConstant(stmt.stmt_index)) << "Expected false but fail at " << stmt.stmt_index;
		}
	}

	TEST_F(AffectsBipPreprocessorTest, evaluateConstantAndConstant) {
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

	TEST_F(AffectsBipPreprocessorTest, evaluateSynonymAndSynonym) {
		auto v1 = processor.evaluateSynonymAndSynonym();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_EQ(v1, expected_pairs);
	}

	TEST_F(AffectsBipPreprocessorTest, evaluateWildAndSynonym) {
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

	TEST_F(AffectsBipPreprocessorTest, evaluateSynonymAndWild) {
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

	TEST_F(AffectsBipPreprocessorTest, evaluateConstantAndSynonym) {
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

	TEST_F(AffectsBipPreprocessorTest, evaluateSynonymAndConstant) {
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

	TEST_F(AffectsBipPreprocessorTest, combinations) {
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
			EXPECT_TRUE(processor.evaluateConstantAndWild(stmt.stmt_index)) << "Expected true but fail at " << stmt.stmt_index;
		}
		for (auto& stmt : false_list) {
			EXPECT_FALSE(processor.evaluateConstantAndWild(stmt.stmt_index)) << "Expected false but fail at " << stmt.stmt_index;
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

	TEST_F(AffectsBipPreprocessorTest, reset) {
		processor.evaluateSynonymAndSynonym();
		EXPECT_TRUE(processor.isFullyPopulated());
		EXPECT_FALSE(processor.isCacheEmpty());
		processor.reset();
		EXPECT_FALSE(processor.isFullyPopulated());
		EXPECT_TRUE(processor.isCacheEmpty());
	}
}