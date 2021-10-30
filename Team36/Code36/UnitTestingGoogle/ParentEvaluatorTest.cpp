#include "pch.h"

#include "ParentEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class ParentEvaluatorTest : public testing::Test {
	protected:
		ParentEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addParent(1, 2);
			PKB::getInstance().addParent(2, 3);
		}

		PKBAdapter pkb;
		ParentEvaluator parentEvaluator;

		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		Entity e5 = { STMT, "5" };

		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };
	};
	TEST_F(ParentEvaluatorTest, evaluateWildAndWild) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_FALSE(parentEvaluator.evaluateWildAndWild());
		PKB::getInstance().addParent(1, 2);
		EXPECT_TRUE(parentEvaluator.evaluateWildAndWild());
	}

	TEST_F(ParentEvaluatorTest, evaluateConstantAndConstant) {
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(3, 4);

		EXPECT_TRUE(parentEvaluator.evaluateConstantAndConstant(e1, e2));
		EXPECT_TRUE(parentEvaluator.evaluateConstantAndConstant(e1, e3));
		EXPECT_TRUE(parentEvaluator.evaluateConstantAndConstant(e2, e3));
		EXPECT_TRUE(parentEvaluator.evaluateConstantAndConstant(e3, e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e1, e1));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e1, e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e1, e5));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e2, e1));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e2, e2));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e2, e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e2, e5));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e3, e1));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e3, e2));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e3, e3));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e3, e5));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e4, e1));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e4, e2));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e4, e3));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e4, e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e4, e5));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e5, e1));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e5, e2));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e5, e3));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e5, e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndConstant(e5, e5));
	}

	TEST_F(ParentEvaluatorTest, evaluateConstantAndWild) {
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(3, 4);

		EXPECT_TRUE(parentEvaluator.evaluateConstantAndWild(e1));
		EXPECT_TRUE(parentEvaluator.evaluateConstantAndWild(e2));
		EXPECT_TRUE(parentEvaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndWild(e4));
		EXPECT_FALSE(parentEvaluator.evaluateConstantAndWild(e5));
	}

	TEST_F(ParentEvaluatorTest, evaluateWildAndConstant) {
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(3, 4);

		EXPECT_FALSE(parentEvaluator.evaluateWildAndConstant(e1));
		EXPECT_TRUE(parentEvaluator.evaluateWildAndConstant(e2));
		EXPECT_TRUE(parentEvaluator.evaluateWildAndConstant(e3));
		EXPECT_TRUE(parentEvaluator.evaluateWildAndConstant(e4));
		EXPECT_FALSE(parentEvaluator.evaluateWildAndConstant(e5));
	}

	TEST_F(ParentEvaluatorTest, evaluateSynonymAndSynonym) {

		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getAllParentRelation();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p1, p2} };
		left = { IF, Synonym{"a"} };
		right = { WHILE, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p2, p3} };
		left = { WHILE, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(ParentEvaluatorTest, evaluateWildAndSynonym) {

		std::vector<StmtInfo> v = pkb.getChild();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);

		v = { p3 };
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);

		v = { };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateWildAndSynonym(header), t);
	}

	TEST_F(ParentEvaluatorTest, evaluateSynonymAndWild) {

		std::vector<StmtInfo> v = pkb.getParent();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);

		v = { p1 };
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);

		v = { };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(ParentEvaluatorTest, evaluateConstantAndSynonym) {

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { PRINT, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateConstantAndSynonym(match, header), t);
	}

	TEST_F(ParentEvaluatorTest, evaluateSynonymAndConstant) {

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.evaluateSynonymAndConstant(header, match), t);
	}
}