#include "pch.h"

#include "PQL/QueryEvaluator/RelationEvaluator/ModifiesSEvaluator.h"
#include "PKB/PKB.h"

namespace UnitTesting {
	class ModifiesSEvaluatorTest : public testing::Test {
	protected:
		ModifiesSEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addVariable(x);
			PKB::getInstance().addVariable(y);
			PKB::getInstance().addVariable(z);
			PKB::getInstance().addModifiesS(1, x);
			PKB::getInstance().addModifiesS(3, y);
		}

		PKBAdapter pkb;
		ModifiesSEvaluator evaluator;

		StmtInfo p1{ 1, STMT_READ };
		StmtInfo p2{ 2, STMT_PRINT };
		StmtInfo p3{ 3, STMT_IF };
		StmtInfo p4{ 4, STMT_PRINT };

		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "4" };

		var_name x = "x";
		var_name y = "y";
		var_name z = "z";

		Entity v1 = { VARIABLE, x };
		Entity v2 = { VARIABLE, y };
		Entity v3 = { VARIABLE, z };
	};
	TEST_F(ModifiesSEvaluatorTest, evaluateWildAndWild) {
		EXPECT_THROW(evaluator.evaluateWildAndWild(), std::invalid_argument);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateConstantAndConstant) {
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e2, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, v1));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e3, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e3, v3));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e4, v3));
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateConstantAndWild) {
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e2));
		EXPECT_TRUE(evaluator.evaluateConstantAndWild(e3));
		EXPECT_FALSE(evaluator.evaluateConstantAndWild(e4));
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateWildAndConstant) {
		EXPECT_THROW(evaluator.evaluateWildAndConstant(v1), std::invalid_argument);
		EXPECT_THROW(evaluator.evaluateWildAndConstant(v2), std::invalid_argument);
		EXPECT_THROW(evaluator.evaluateWildAndConstant(v3), std::invalid_argument);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateSynonymAndSynonym) {
		std::vector<std::pair<StmtInfo, var_name>> v = pkb.getModifiesSRelation();
		Entity left = { STMT, Synonym{"a"} };
		Entity right = { VARIABLE, Synonym{"b"} };
		std::pair<Entity, Entity> header = { left, right };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p1, x} };
		left = { READ, Synonym{"a"} };
		right = { VARIABLE, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { {p3, y} };
		left = { IF, Synonym{"a"} };
		right = { VARIABLE, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		v = { };
		left = { WHILE, Synonym{"a"} };
		right = { READ, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);

		left = { PRINT, Synonym{"a"} };
		right = { IF, Synonym{"b"} };
		header = { left, right };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(left, right), t);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateWildAndSynonym) {
		Entity header = { VARIABLE, Synonym{"a"} };
		EXPECT_THROW(evaluator.evaluateWildAndSynonym(header), std::invalid_argument);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateSynonymAndWild) {
		std::vector<StmtInfo> v = pkb.getModifiesS();
		Entity header = { STMT, Synonym{"a"} };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { p1 };
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { p3 };
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);

		v = { };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndWild(header), t);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateConstantAndSynonym) {
		std::vector<var_name> v = { x };
		Entity header = { VARIABLE, Synonym{"a"} };
		Entity match = { STMT, "1" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { y };
		match = { STMT, "3" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		match = { IF, "3" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		match = { WHILE, "3" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);

		v = { };
		header = { VARIABLE, Synonym{"a"} };
		match = { STMT, "2" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym(match, header), t);
	}

	TEST_F(ModifiesSEvaluatorTest, evaluateSynonymAndConstant) {
		std::vector<StmtInfo> v = { p3 };
		Entity header = { STMT, Synonym{"a"} };
		Entity match = { VARIABLE, "y" };
		ResultTable t(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = {};
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { p1 };
		header = { STMT, Synonym{"a"} };
		match = { VARIABLE, "x" };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = {};
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);

		v = { };
		match = { VARIABLE, "z" };
		header = { WHILE, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { CALL, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { IF, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { ASSIGN, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { READ, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
		header = { PRINT, Synonym{"a"} };
		t = ResultTable(header, v);
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, match), t);
	}
}