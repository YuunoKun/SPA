#include "pch.h"

#include "QueryOptimizer.h"

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

}