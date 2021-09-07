#include "pch.h"

#include "FollowsEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class FollowsEvaluatorTest : public testing::Test {
	protected:
		FollowsEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();
		}

		PKBAdapter pkb;
		FollowsEvaluator evaluator;
	};

	TEST_F(FollowsEvaluatorTest, haveRelation) {
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_FALSE(evaluator.haveRelation());
		PKB::getInstance().addFollows(1, 2);
		EXPECT_TRUE(evaluator.haveRelation());
	}

	TEST_F(FollowsEvaluatorTest, isRelation) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_TRUE(evaluator.isRelation(e1, e2));
		EXPECT_TRUE(evaluator.isRelation(e2, e3));
		EXPECT_FALSE(evaluator.isRelation(e1, e1));
		EXPECT_FALSE(evaluator.isRelation(e1, e3));
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

	TEST_F(FollowsEvaluatorTest, haveRelationAtRight) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_TRUE(evaluator.haveRelationAtRight(e1));
		EXPECT_TRUE(evaluator.haveRelationAtRight(e2));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e3));
		EXPECT_FALSE(evaluator.haveRelationAtRight(e4));
	}

	TEST_F(FollowsEvaluatorTest, haveRelationAtLeft) {
		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);
		EXPECT_FALSE(evaluator.haveRelationAtLeft(e1));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e2));
		EXPECT_TRUE(evaluator.haveRelationAtLeft(e3));
		EXPECT_FALSE(evaluator.haveRelationAtLeft(e4));
	}

	TEST_F(FollowsEvaluatorTest, getRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getFollows();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { {p1, p2} };
		left = { READ, Synonym{"a"} };
		right = { PRINT, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { {p2, p3} };
		left = { PRINT, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);

		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelations(left, right), t);
	}

	TEST_F(FollowsEvaluatorTest, getRightRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

		std::vector<StmtInfo> v = pkb.getFollowing();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { p3 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRightRelations(header), t);
	}

	TEST_F(FollowsEvaluatorTest, getLeftRelations) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

		std::vector<StmtInfo> v = pkb.getFollowed();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { p1 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getLeftRelations(header), t);
	}

	TEST_F(FollowsEvaluatorTest, getRelationMatchLeft) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchLeft(match, header), t);
	}

	TEST_F(FollowsEvaluatorTest, getRelationMatchRight) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addFollows(1, 2);
		PKB::getInstance().addFollows(2, 3);

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.getRelationMatchRight(header, match), t);
		header = { IF, Synonym{"a"} };
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