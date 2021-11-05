#include "pch.h"

#include "AffectsBipEvaluator.h"
#include "PKB.h"
#include "CFGRelationsManager.h"

namespace UnitTesting {
	class AffectsBipEvaluatorTest : public testing::Test {
	protected:
		AffectsBipEvaluatorTest() {
			cfg->add(1);
			cfg->add(2);
			cfg->add(3);
			cfg->add(4);
		}

		~AffectsBipEvaluatorTest() {
			delete cfg;
		}

		virtual void SetUp() override {
			PKB::getInstance().resetCache();

			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);
			PKB::getInstance().addStmt(STMT_ASSIGN);

			var_name v1 = "x";
			var_name v2 = "y";
			PKB::getInstance().addVariable(v1);
			PKB::getInstance().addVariable(v2);

			proc_name p = "p";
			PKB::getInstance().addProcedure(p);
			PKB::getInstance().addProcContains(p, 1);
			PKB::getInstance().addProcContains(p, 2);
			PKB::getInstance().addProcContains(p, 3);
			PKB::getInstance().addProcContains(p, 4);

			PKB::getInstance().addNext(1, 2);
			PKB::getInstance().addNext(2, 3);

			PKB::getInstance().addNext(1, 2);
			PKB::getInstance().addVariable(v1);
			PKB::getInstance().addModifiesS(1, v1);
			PKB::getInstance().addUsesS(2, v1);
			PKB::getInstance().addModifiesS(2, v2);
			PKB::getInstance().addUsesS(3, v2);
			PKB::getInstance().addCFGBip(cfg);
			pkb.getRelationManager().reset();
		}

		CFG* cfg = new CFG();

		PKBAdapter pkb;
		AffectsBipEvaluator evaluator;

		StmtInfo p1{ 1, STMT_ASSIGN };
		StmtInfo p2{ 2, STMT_ASSIGN };
		StmtInfo p3{ 3, STMT_ASSIGN };
		StmtInfo p4{ 4, STMT_ASSIGN };

		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };

		var_name v1 = "x";
		var_name v2 = "y";

		proc_name p = "p";
	};

	TEST_F(AffectsBipEvaluatorTest, evaluateWildAndWild) {
		PKB::getInstance().resetCache();
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addStmt(STMT_ASSIGN);
		PKB::getInstance().addProcedure(p);
		PKB::getInstance().addProcContains(p, 1);
		PKB::getInstance().addProcContains(p, 2);
		PKB::getInstance().addProcContains(p, 3);
		PKB::getInstance().addProcContains(p, 4);
		PKB::getInstance().addCFGBip(cfg);
		EXPECT_FALSE(evaluator.evaluateWildAndWild());
		PKB::getInstance().addNext(1, 2);
		PKB::getInstance().addVariable(v1);
		PKB::getInstance().addModifiesS(1, v1);
		PKB::getInstance().addUsesS(2, v1);
		pkb.getRelationManager().reset();
		EXPECT_TRUE(evaluator.evaluateWildAndWild());
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateConstantAndConstant) {
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, e2));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e2, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e1));
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

	TEST_F(AffectsBipEvaluatorTest, evaluateConstantAndWild) {
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e1));
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e4));
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateWildAndConstant) {
		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e1));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e2));
		EXPECT_TRUE(evaluator.evaluateWildAndConstant(e3));
		EXPECT_FALSE(evaluator.evaluateWildAndConstant(e4));
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateSynonymAndSynonym) {
		std::vector<std::pair<StmtInfo, StmtInfo>> v = pkb.getRelationManager().getAllAffectsBipRelation();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { STMT, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { ASSIGN, Synonym{"a"} };
		right = { ASSIGN, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { };
		left = { READ, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { PRINT, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { READ, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { IF, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateWildAndSynonym) {
		std::vector<StmtInfo> v = pkb.getRelationManager().getAffectedBip();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateWildAndSynonym(header), t);

		header = { ASSIGN, Synonym{"a"} };
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
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateSynonymAndWild) {
		std::vector<StmtInfo> v = pkb.getRelationManager().getAffectingBip();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { };
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateConstantAndSynonym) {
		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { ASSIGN, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { PROG_LINE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { p3 };
		header = { STMT, Synonym{"a"} };
		match = { ASSIGN, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		header = { PRINT, Synonym{"a"} };
		match = { ASSIGN, "1" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		header = { READ, Synonym{"a"} };
		match = { ASSIGN, "1" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		header = { READ, Synonym{"a"} };
		match = { ASSIGN, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

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
	}

	TEST_F(AffectsBipEvaluatorTest, evaluateSynonymAndConstant) {
		std::vector<StmtInfo> v = { p2 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { ASSIGN, "3" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { PROG_LINE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { ASSIGN, "2" };
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
	}
}