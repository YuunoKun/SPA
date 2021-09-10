#include "pch.h"

#include "ParentTEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class ParentTEvaluatorTest : public testing::Test {
	protected:
		ParentTEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		ParentTEvaluator evaluator;
	};

	TEST_F(ParentTEvaluatorTest, haveRelation) {
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_FALSE(evaluator.haveRelation());
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().generateParentT();
		EXPECT_TRUE(evaluator.haveRelation());
	}

	TEST_F(ParentTEvaluatorTest, isRelation) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();
		EXPECT_TRUE(evaluator.isRelation(e1, e2));
		EXPECT_TRUE(evaluator.isRelation(e1, e3));
		EXPECT_TRUE(evaluator.isRelation(e2, e3));
		EXPECT_FALSE(evaluator.isRelation(e1, e1));
		EXPECT_FALSE(evaluator.isRelation(e1, e4));
		EXPECT_FALSE(evaluator.isRelation(e2, e1));
		EXPECT_FALSE(evaluator.isRelation(e2, e2));
		EXPECT_FALSE(evaluator.isRelation(e2, e4));
		EXPECT_FALSE(evaluator.isRelation(e3, e1));
		EXPECT_FALSE(evaluator.isRelation(e3, e2));
		EXPECT_FALSE(evaluator.isRelation(e3, e3));
		EXPECT_FALSE(evaluator.isRelation(e3, e4));
		EXPECT_FALSE(evaluator.isRelation(e4, e1));
		EXPECT_FALSE(evaluator.isRelation(e4, e2));
		EXPECT_FALSE(evaluator.isRelation(e4, e3));
		EXPECT_FALSE(evaluator.isRelation(e4, e4));
	}

	TEST_F(ParentTEvaluatorTest, haveRelationAtRight) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();
		EXPECT_TRUE(evaluator.haveRelationAtRight(e1));
		EXPECT_TRUE(evaluator.haveRelationAtRight(e2));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e3));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e4));
	}

	TEST_F(ParentTEvaluatorTest, haveRelationAtLeft) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();
		EXPECT_FALSE(evaluator.haveRelationAtLeft(e1));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e2));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e3));
		EXPECT_FALSE(evaluator.haveRelationAtLeft(e4));
	}

	TEST_F(ParentTEvaluatorTest, getRelations) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();

		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getAllParentTRelation();

		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { {p1, p2} };
		left = { IF, Synonym{"a"} };
		right = { WHILE, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { {p1, p3} };
		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { {p2, p3} };
		left = { WHILE, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		left = { WHILE, Synonym{"a"} };
		right = { WHILE, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);
	}

	TEST_F(ParentTEvaluatorTest, getRightRelations) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();

		std::vector<StmtInfo> v = pkb.getChildT();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { p3 };
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
	}

	TEST_F(ParentTEvaluatorTest, getLeftRelations) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();

		std::vector<StmtInfo> v = pkb.getParentT();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { p1 };
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
	}

	TEST_F(ParentTEvaluatorTest, getRelationMatchLeft) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();

		std::vector<StmtInfo> v = { p2, p3 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		match = { STMT, "1" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { p3 };
		header = { IF, Synonym{"a"} };
		match = { STMT, "1" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);


		v = { p3 };
		header = { IF, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		header = { READ, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { PRINT, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
	}

	TEST_F(ParentTEvaluatorTest, getRelationMatchRight) {
		StmtInfo p1{ 1, STMT_IF };
		StmtInfo p2{ 2, STMT_WHILE };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_WHILE);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addParent(1, 2);
		PKB::getInstance().addParent(2, 3);
		PKB::getInstance().generateParentT();

		std::vector<StmtInfo> v = { p1, p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { p1 };
		header = { IF, Synonym{"a"} };
		match = { STMT, "3" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { p2 };
		header = { WHILE, Synonym{"a"} };
		match = { STMT, "3" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { };
		header = { READ, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { PRINT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
	}
}