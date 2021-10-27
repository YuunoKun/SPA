#include <iostream>

#include "pch.h"
#include "PKB.h"
#include <NextTPreprocessor.h>

namespace UnitTesting {
	class NextTPreprocessorTest : public testing::Test {
	protected:

		StmtInfo s1{ 1, STMT_READ };
		StmtInfo s2{ 2, STMT_WHILE };
		StmtInfo s3{ 3, STMT_IF };
		StmtInfo s4{ 4, STMT_PRINT };
		StmtInfo s5{ 5, STMT_READ };
		StmtInfo s6{ 6, STMT_READ };
		StmtInfo s7{ 7, STMT_READ };
		std::vector<StmtInfo> s = { s1, s2, s3, s4, s5, s6, s7 };

		std::vector<StmtInfo> empty_result = {};
		std::vector<std::pair<StmtInfo, StmtInfo>> test_input = {
			{s1, s2}, {s2, s3}, {s3, s4}, {s3, s5}, {s4, s2},
			{s5, s2}, {s2, s6}
		};
		MonotypeRelationTable<StmtInfo> test = MonotypeRelationTable<StmtInfo>(test_input);

		NextTPreprocessor processor = NextTPreprocessor(test, s);

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
			processor.reset();
		}
	};

	TEST_F(NextTPreprocessorTest, evaluateWildAndWild) {
		EXPECT_TRUE(processor.evaluateWildAndWild());
	}

	TEST_F(NextTPreprocessorTest, evaluateConstantAndWild) {
		EXPECT_TRUE(processor.evaluateConstantAndWild(1));
		EXPECT_TRUE(processor.evaluateConstantAndWild(2));
		EXPECT_TRUE(processor.evaluateConstantAndWild(3));
		EXPECT_TRUE(processor.evaluateConstantAndWild(4));
		EXPECT_TRUE(processor.evaluateConstantAndWild(5));
		EXPECT_FALSE(processor.evaluateConstantAndWild(6));
	}

	TEST_F(NextTPreprocessorTest, evaluateWildAndConstant) {
		EXPECT_FALSE(processor.evaluateWildAndConstant(1));
		EXPECT_TRUE(processor.evaluateWildAndConstant(2));
		EXPECT_TRUE(processor.evaluateWildAndConstant(3));
		EXPECT_TRUE(processor.evaluateWildAndConstant(4));
		EXPECT_TRUE(processor.evaluateWildAndConstant(5));
		EXPECT_TRUE(processor.evaluateWildAndConstant(6));
	}

	TEST_F(NextTPreprocessorTest, evaluateConstantAndConstant) {
		EXPECT_FALSE(processor.evaluateConstantAndConstant(1, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(2, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(2, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(2, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(2, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(2, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(2, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(3, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(3, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(3, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(3, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(3, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(3, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(4, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(4, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(4, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(4, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(4, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(4, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(5, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(5, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(5, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(5, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(5, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(5, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 1));
		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 2));
		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 3));
		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 4));
		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 5));
		EXPECT_FALSE(processor.evaluateConstantAndConstant(6, 6));

		EXPECT_FALSE(processor.evaluateConstantAndConstant(1, 1));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 2));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 3));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 4));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 5));
		EXPECT_TRUE(processor.evaluateConstantAndConstant(1, 6));
	}

	TEST_F(NextTPreprocessorTest, evaluateSynonymAndSynonym) {
		auto v1 = processor.evaluateSynonymAndSynonym();
		std::sort(v1.begin(), v1.end());
		EXPECT_EQ(v1, expected_pairs);
		EXPECT_TRUE(processor.isFullyPopulated());
	}

	TEST_F(NextTPreprocessorTest, evaluateWildAndSynonym) {
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
	}

	TEST_F(NextTPreprocessorTest, evaluateSynonymAndWild) {
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
	}

	TEST_F(NextTPreprocessorTest, evaluateConstantAndSynonym) {
		for (auto& stmt : s) {
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
				EXPECT_TRUE(processor.getCalculatedMatrix()[stmt.stmt_index - 1][i]);
				EXPECT_TRUE(processor.isDFSForwardComputed(stmt.stmt_index));
			}
		}
	}

	TEST_F(NextTPreprocessorTest, evaluateSynonymAndConstant) {
		for (auto& stmt : s) {
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
				EXPECT_TRUE(processor.getCalculatedMatrix()[i][stmt.stmt_index - 1]);
				EXPECT_TRUE(processor.isDFSBackwardComputed(stmt.stmt_index));
			}
		}
	}

	TEST_F(NextTPreprocessorTest, reset) {
		for (auto& stmt : s) {
			processor.evaluateSynonymAndConstant(stmt.stmt_index);
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_TRUE(processor.getCalculatedMatrix()[i][stmt.stmt_index - 1]);
				EXPECT_TRUE(processor.isDFSBackwardComputed(stmt.stmt_index));
			}
		}
		for (auto& stmt : s) {
			processor.evaluateConstantAndSynonym(stmt.stmt_index);
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_TRUE(processor.getCalculatedMatrix()[stmt.stmt_index - 1][i]);
				EXPECT_TRUE(processor.isDFSForwardComputed(stmt.stmt_index));
			}
		}
		processor.reset();
		for (auto& stmt : s) {
			for (int i = 0; i < PKB::getInstance().getStmts().size(); i++) {
				EXPECT_FALSE(processor.getCalculatedMatrix()[i][stmt.stmt_index - 1]);
				EXPECT_FALSE(processor.isDFSBackwardComputed(stmt.stmt_index));
				EXPECT_FALSE(processor.getCalculatedMatrix()[stmt.stmt_index - 1][i]);
				EXPECT_FALSE(processor.isDFSForwardComputed(stmt.stmt_index));
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