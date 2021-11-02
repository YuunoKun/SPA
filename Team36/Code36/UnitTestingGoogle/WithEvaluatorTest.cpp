#include "pch.h"

#include "WithEvaluator.h"
#include "PKB.h"

namespace UnitTesting {
	class WithEvaluatorTest : public testing::Test {
	protected:
		WithEvaluatorTest() {
			PKB::getInstance().resetCache();
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_WHILE);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_IF);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_PRINT);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_READ);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addStmt(STMT_CALL);
			PKB::getInstance().addVariable(s1);
			PKB::getInstance().addVariable(s2);
			PKB::getInstance().addProcedure(s1);
			PKB::getInstance().addProcedure(s2);
			PKB::getInstance().addUsesS(5, s1);
			PKB::getInstance().addUsesS(6, s2);
			PKB::getInstance().addModifiesS(7, s1);
			PKB::getInstance().addModifiesS(8, s2);
			PKB::getInstance().addCallsS(9, s1);
			PKB::getInstance().addCallsS(10, s2);
			PKB::getInstance().addConstant(1);
			PKB::getInstance().addConstant(2);
		}

		PKBAdapter pkb;
		WithEvaluator evaluator;

		Entity e1 = { STMT, "1" };
		Entity e2 = { STMT, "2" };
		Entity e3 = { STMT, "3" };
		Entity e4 = { STMT, "1" };

		std::string s1 = "x";
		std::string s2 = "y";
		std::string s3 = "z";
	};

	TEST_F(WithEvaluatorTest, evaluateWildAndWild) {
		EXPECT_THROW(evaluator.evaluateWildAndWild(), std::invalid_argument);
	}

	TEST_F(WithEvaluatorTest, evaluateConstantAndConstant) {
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, e1));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e2));
		EXPECT_FALSE(evaluator.evaluateConstantAndConstant(e1, e3));
		EXPECT_TRUE(evaluator.evaluateConstantAndConstant(e1, e4));
	}

	TEST_F(WithEvaluatorTest, evaluateConstantAndWild) {
		Entity v1 = { VARIABLE, "x" };
		EXPECT_THROW(evaluator.evaluateConstantAndWild(v1), std::invalid_argument);
	}

	TEST_F(WithEvaluatorTest, evaluateWildAndConstant) {
		Entity v1 = { VARIABLE, "x" };
		EXPECT_THROW(evaluator.evaluateWildAndConstant(v1), std::invalid_argument);
	}

	TEST_F(WithEvaluatorTest, evaluateSynonymAndSynonym) {
		std::vector<std::string> v = { "1", "2"};
		std::vector<Entity> headers = { { WHILE, Synonym{"a"} }, { WHILE, Synonym{"a"} } };
		EXPECT_EQ(evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]), ResultTable(headers[0], v));

		headers = { { WHILE, Synonym{ "a" } }, { IF, Synonym{"b"} } };
		std::list<std::string> r = {  };
		std::list<std::string> out; 
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[0], out);
		EXPECT_EQ(out, r);

		headers = { { PRINT, Synonym{ "a" }, VAR_NAME }, { READ, Synonym{"b"} , VAR_NAME } };
		r = { "5", "6" };
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[0], out);
		EXPECT_EQ(out, r);
		r = { "7", "8" };
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[1], out);
		EXPECT_EQ(out, r);

		headers = { { PROCEDURE, Synonym{ "a" }, PROC_NAME }, { CALL, Synonym{"b"} , PROC_NAME } };
		r = { s1, s2 };
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[0], out);
		EXPECT_EQ(out, r);
		r = { "10", "9" };
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[1], out);
		EXPECT_EQ(out, r);

		headers = { { WHILE, Synonym{ "a" } }, { CONSTANT, Synonym{"b"} } };
		r = { "1", "2"};
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[0], out);
		EXPECT_EQ(out, r);
		r = { "1", "2" };
		out.clear();
		evaluator.evaluateSynonymAndSynonym(headers[0], headers[1]).getEntityResult(headers[1], out);
		EXPECT_EQ(out, r);

	}

	TEST_F(WithEvaluatorTest, evaluateWildAndSynonym) {
		Entity header = { STMT, Synonym{"a"} };
		EXPECT_THROW(evaluator.evaluateWildAndSynonym(header), std::invalid_argument);
	}

	TEST_F(WithEvaluatorTest, evaluateSynonymAndWild) {
		Entity header = { STMT, Synonym{"a"} };
		EXPECT_THROW(evaluator.evaluateWildAndSynonym(header), std::invalid_argument);
	}

	TEST_F(WithEvaluatorTest, evaluateSynonymAndConstant) {
		std::vector<std::string> v = { "1" };
		Entity header = { STMT, Synonym{"a"} };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ STMT, "1" }, header), ResultTable(header, v));
		v = { "2" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { STMT, "2" }), ResultTable(header, v));

		header = { WHILE, Synonym{"a"} };
		v = { "1" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ WHILE, "1" }, header), ResultTable(header, v));
		v = { "2" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { WHILE, "2" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { WHILE, "3" }), ResultTable(header, v));

		header = { IF, Synonym{"a"} };
		v = { "3" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ IF, "3" }, header), ResultTable(header, v));
		v = { "4" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { IF, "4" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { IF, "1" }), ResultTable(header, v));

		header = { PRINT, Synonym{"a"} };
		v = { "5" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ PRINT, "5" }, header), ResultTable(header, v));
		v = { "6" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PRINT, "6" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PRINT, "1" }), ResultTable(header, v));

		header = { PRINT, Synonym{"a"}, VAR_NAME };
		v = { "5" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ PRINT, s1, VAR_NAME }, header), ResultTable(header, v));
		v = { "6" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PRINT, s2, VAR_NAME }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PRINT, s3, VAR_NAME }), ResultTable(header, v));

		header = { READ, Synonym{"a"} };
		v = { "7" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ READ, "7" }, header), ResultTable(header, v));
		v = { "8" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { READ, "8" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { READ, "1" }), ResultTable(header, v));

		header = { READ, Synonym{"a"}, VAR_NAME };
		v = { "7" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ READ, s1, VAR_NAME }, header), ResultTable(header, v));
		v = { "8" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { READ, s2, VAR_NAME }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { READ, s3, VAR_NAME }), ResultTable(header, v));

		header = { CALL, Synonym{"a"} };
		v = { "9" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ CALL, "9" }, header), ResultTable(header, v));
		v = { "10" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { CALL, "10" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { WHILE, "1" }), ResultTable(header, v));

		header = { CALL, Synonym{"a"}, PROC_NAME };
		v = { "9" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ CALL, s1, PROC_NAME }, header), ResultTable(header, v));
		v = { "10" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { CALL, s2, PROC_NAME }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { CALL, s3, PROC_NAME }), ResultTable(header, v));

		header = { VARIABLE, Synonym{"a"} };
		v = { s1 };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ VARIABLE, s1 }, header), ResultTable(header, v));
		v = { s2 };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { VARIABLE, s2 }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { VARIABLE, s3 }), ResultTable(header, v));

		header = { PROCEDURE, Synonym{"a"} };
		v = { s1 };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ PROCEDURE, s1 }, header), ResultTable(header, v));
		v = { s2 };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PROCEDURE, s2 }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { PROCEDURE, s3 }), ResultTable(header, v));

		header = { CONSTANT, Synonym{"a"} };
		v = { "1" };
		EXPECT_EQ(evaluator.evaluateConstantAndSynonym({ CONSTANT, "1" }, header), ResultTable(header, v));
		v = { "2" };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { CONSTANT, "2" }), ResultTable(header, v));
		v = { };
		EXPECT_EQ(evaluator.evaluateSynonymAndConstant(header, { CONSTANT, "3" }), ResultTable(header, v));


	}
}