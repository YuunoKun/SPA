#include "pch.h"

#include "QueryOptimizer.h"
#include <PKB.h>

namespace UnitTesting {
	TEST(QueryOptimizer, optimizeClausesOrder) {
		QueryOptimizer optimizer;
		RelRef rel_ref_no_synonym(FOLLOWS, { STMT, "1" }, { STMT, "2" });
		RelRef rel_ref_one_synonym(PARENT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef rel_ref_two_synonym(FOLLOWS_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		Pattern pattern_no_synonym({ ASSIGN, "1" }, { WILD }, "", true);
		Pattern pattern_one_synonym({ ASSIGN, Synonym("a") }, { WILD }, "", true);
		Pattern patternTwoSynonym({ ASSIGN, Synonym("a") }, { VARIABLE, Synonym("b")}, "", true);

		std::vector<Clause> unordered_clauses, ordered_clauses;
		unordered_clauses.push_back({ rel_ref_one_synonym });
		unordered_clauses.push_back({ rel_ref_two_synonym });
		unordered_clauses.push_back({ rel_ref_no_synonym });

		ordered_clauses.push_back({ rel_ref_no_synonym });
		ordered_clauses.push_back({ rel_ref_one_synonym });
		ordered_clauses.push_back({ rel_ref_two_synonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ patternTwoSynonym });
		unordered_clauses.push_back({ pattern_one_synonym });
		unordered_clauses.push_back({ pattern_no_synonym });

		ordered_clauses.push_back({ pattern_no_synonym });
		ordered_clauses.push_back({ pattern_one_synonym });
		ordered_clauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);

		unordered_clauses.clear();
		ordered_clauses.clear();
		unordered_clauses.push_back({ rel_ref_one_synonym });
		unordered_clauses.push_back({ rel_ref_two_synonym });
		unordered_clauses.push_back({ rel_ref_no_synonym });
		unordered_clauses.push_back({ patternTwoSynonym });
		unordered_clauses.push_back({ pattern_one_synonym });
		unordered_clauses.push_back({ pattern_no_synonym });

		ordered_clauses.push_back({ rel_ref_no_synonym });
		ordered_clauses.push_back({ pattern_no_synonym });
		ordered_clauses.push_back({ rel_ref_one_synonym });
		ordered_clauses.push_back({ pattern_one_synonym });
		ordered_clauses.push_back({ rel_ref_two_synonym });
		ordered_clauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(optimizer.optimizeClausesOrder(unordered_clauses), ordered_clauses);
	}

	TEST(QueryOptimizer, checkConstantExist) {
		PKB::getInstance().resetCache();
		QueryOptimizer optimizer;

		RelRef stmt_constant_synonym1(FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" });
		RelRef stmt_constant_synonym2(FOLLOWS, { IF, "1"}, {STMT, Synonym("a") });
		RelRef if_constant_synonym(FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" });
		Pattern stmt_constant_synonym3({ ASSIGN, "1" }, { WILD }, "", true);

		std::vector<Clause> clauses;
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { STMT, "1" } } });
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "1" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "2" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));


		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { READ, "3" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { READ, "4" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_READ);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { PRINT, "5" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_PRINT);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { PRINT, "6" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_PRINT);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { CALL, "7" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_CALL);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { CALL, "8" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_CALL);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { WHILE, "9" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_WHILE);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { WHILE, "10" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_WHILE);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "11" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { IF, "12" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_IF);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { ASSIGN, "13" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_ASSIGN);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));
		clauses.push_back({ { FOLLOWS, { STMT, Synonym("a") }, { ASSIGN, "14" } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addStmt(STMT_ASSIGN);
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ {{ ASSIGN, Synonym("a") }, { VARIABLE ,"x"}, "", true} });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addVariable("x");
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));

		clauses.push_back({ { CALLS, { PROCEDURE, "x" }, { PROCEDURE, Synonym("a") } } });
		EXPECT_FALSE(optimizer.checkAllConstantExist(clauses));
		PKB::getInstance().addProcedure("x");
		EXPECT_TRUE(optimizer.checkAllConstantExist(clauses));


	}

}