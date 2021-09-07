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
		}

		PKBAdapter pkb;
		ParentEvaluator parentEvaluator;
	};
	TEST_F(ParentEvaluatorTest, haveRelation) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_FALSE(parentEvaluator.haveRelation());
		PKB::getInstance().addParent(1, 2);
		EXPECT_TRUE(parentEvaluator.haveRelation());
	}

	TEST_F(ParentEvaluatorTest, isRelation) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		Entity e5 = { STMT, "5" };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		EXPECT_TRUE(parentEvaluator.isRelation(e1, e2));
		EXPECT_TRUE(parentEvaluator.isRelation(e1, e3));
		EXPECT_TRUE(parentEvaluator.isRelation(e2, e3));
		EXPECT_TRUE(parentEvaluator.isRelation(e3, e4));
		EXPECT_FALSE(parentEvaluator.isRelation(e1, e1));
		EXPECT_FALSE(parentEvaluator.isRelation(e1, e4));
		EXPECT_FALSE(parentEvaluator.isRelation(e1, e5));
		EXPECT_FALSE(parentEvaluator.isRelation(e2, e1));
		EXPECT_FALSE(parentEvaluator.isRelation(e2, e2));
		EXPECT_FALSE(parentEvaluator.isRelation(e2, e4));
		EXPECT_FALSE(parentEvaluator.isRelation(e2, e5));
		EXPECT_FALSE(parentEvaluator.isRelation(e3, e1));
		EXPECT_FALSE(parentEvaluator.isRelation(e3, e2));
		EXPECT_FALSE(parentEvaluator.isRelation(e3, e3));
		EXPECT_FALSE(parentEvaluator.isRelation(e3, e5));
		EXPECT_FALSE(parentEvaluator.isRelation(e4, e1));
		EXPECT_FALSE(parentEvaluator.isRelation(e4, e2));
		EXPECT_FALSE(parentEvaluator.isRelation(e4, e3));
		EXPECT_FALSE(parentEvaluator.isRelation(e4, e4));
		EXPECT_FALSE(parentEvaluator.isRelation(e4, e5));
		EXPECT_FALSE(parentEvaluator.isRelation(e5, e1));
		EXPECT_FALSE(parentEvaluator.isRelation(e5, e2));
		EXPECT_FALSE(parentEvaluator.isRelation(e5, e3));
		EXPECT_FALSE(parentEvaluator.isRelation(e5, e4));
		EXPECT_FALSE(parentEvaluator.isRelation(e5, e5));
	}

	TEST_F(ParentEvaluatorTest, haveRelationAtRight) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		Entity e5 = { STMT, "5" };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		EXPECT_TRUE(parentEvaluator.haveRelationAtRight(e1));
		EXPECT_TRUE(parentEvaluator.haveRelationAtRight(e2));
		EXPECT_TRUE(parentEvaluator.haveRelationAtRight(e3));
		EXPECT_FALSE(parentEvaluator.haveRelationAtRight(e4));
		EXPECT_FALSE(parentEvaluator.haveRelationAtRight(e5));
	}

	TEST_F(ParentEvaluatorTest, haveRelationAtLeft) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		Entity e5 = { STMT, "5" };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(1, 3);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().addParent(3, 4);
		EXPECT_FALSE(parentEvaluator.haveRelationAtLeft(e1));
		EXPECT_TRUE(parentEvaluator.haveRelationAtLeft(e2));
		EXPECT_TRUE(parentEvaluator.haveRelationAtLeft(e3));
		EXPECT_TRUE(parentEvaluator.haveRelationAtLeft(e4));
		EXPECT_FALSE(parentEvaluator.haveRelationAtLeft(e5));
	}

	TEST_F(ParentEvaluatorTest, getRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);

		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getAllParentRelation();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.getRelations(left, right), t);

		v = { {p1, p2} };
		left = { READ, Synonym{"a"} };
		right = { PRINT, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelations(left, right), t);

		v = { {p2, p3} };
		left = { PRINT, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelations(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelations(left, right), t);

		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelations(left, right), t);
	}

	TEST_F(ParentEvaluatorTest, getRightRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);

		std::vector<StmtInfo> v = pkb.getChild();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);

		v = { p3 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRightRelations(header), t);
	}

	TEST_F(ParentEvaluatorTest, getLeftRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);

		std::vector<StmtInfo> v = pkb.getParent();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);

		v = { p1 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getLeftRelations(header), t);
	}

	TEST_F(ParentEvaluatorTest, getRelationMatchLeft) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchLeft(match, header), t);
	}

	TEST_F(ParentEvaluatorTest, getRelationMatchRight) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(parentEvaluator.getRelationMatchRight(header, match), t);
	}
}