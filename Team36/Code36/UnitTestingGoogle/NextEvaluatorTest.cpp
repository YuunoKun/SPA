#include "pch.h"

#include "PQL/QueryEvaluator/RelationEvaluator/NextEvaluator.h"
#include "PKB/PKB.h"

namespace UnitTesting {
	class NextEvaluatorTest : public testing::Test {
	protected:
		NextEvaluatorTest() {
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();

			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addNext(1, 2);
			PKB::getInstance().addNext(2, 3);
		}

		PKBAdapter pkb;
		NextEvaluator evaluator;

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };
	};

	TEST_F(NextEvaluatorTest, evaluateWildAndWild) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_FALSE(evaluator.evaluateWildAndWild());
		PKB::getInstance().addNext(1, 2);
		EXPECT_TRUE(evaluator.evaluateWildAndWild());
	}

	TEST_F(NextEvaluatorTest, evaluateConstantAndConstant) {
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, e2));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e2, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, e4));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, e4));
	}

	TEST_F(NextEvaluatorTest, evaluateConstantAndWild) {
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e1));
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e4));
	}

	TEST_F(NextEvaluatorTest, evaluateWildAndConstant) {
		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e1));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e2));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e3));
		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e4));
	}

	TEST_F(NextEvaluatorTest, evaluateSynonymAndSynonym) {
		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getAllNextRelation();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p1, p2} };
		left = { READ, Synonym{"a"} };
		right = { PRINT, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p2, p3} };
		left = { PRINT, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(NextEvaluatorTest, evaluateWildAndSynonym) {
		std::vector<StmtInfo> v = pkb.getNext();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);

		v = { p3 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);
	}

	TEST_F(NextEvaluatorTest, evaluateSynonymAndWild) {
		std::vector<StmtInfo> v = pkb.getPrevious();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { p2 };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { p1 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(NextEvaluatorTest, evaluateConstantAndSynonym) {

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
	}

	TEST_F(NextEvaluatorTest, evaluateSynonymAndConstant) {
		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_READ };
		StmtInfo p4{ 4, STMT_IF };

		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_PRINT);
		PKB::getInstance().addStmt(STMT_READ);
		PKB::getInstance().addStmt(STMT_IF);
		PKB::getInstance().addNext(1, 2);
		PKB::getInstance().addNext(2, 3);

		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { STMT, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		match = { IF, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		match = { WHILE, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
	}
}