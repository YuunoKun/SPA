#include "pch.h"

#include "QueryOptimizer.h"

namespace UnitTesting {
	TEST(QueryOptimizer, optimizeClausesOrder) {

		RelRef relRefNoSynonym(FOLLOWS, { STMT, "1" }, { STMT, "2" });
		RelRef relRefOneSynonym(PARENT, { STMT, Synonym("a") }, { STMT, "2" });
		RelRef relRefTwoSynonym(FOLLOWS_T, { STMT, Synonym("a") }, { STMT, Synonym("b") });
		Pattern patternNoSynonym({ ASSIGN, "1" }, { WILD }, "", true);
		Pattern patternOneSynonym({ ASSIGN, Synonym("a") }, { WILD }, "", true);
		Pattern patternTwoSynonym({ ASSIGN, Synonym("a") }, { VARIABLE, Synonym("b")}, "", true);

		std::vector<Clause> unorderedClauses, orderedClauses;
		unorderedClauses.push_back({ relRefOneSynonym });
		unorderedClauses.push_back({ relRefTwoSynonym });
		unorderedClauses.push_back({ relRefNoSynonym });

		orderedClauses.push_back({ relRefNoSynonym });
		orderedClauses.push_back({ relRefOneSynonym });
		orderedClauses.push_back({ relRefTwoSynonym });
		EXPECT_EQ(QueryOptimizer::optimizeClausesOrder(unorderedClauses), orderedClauses);

		unorderedClauses.clear();
		orderedClauses.clear();
		unorderedClauses.push_back({ patternTwoSynonym });
		unorderedClauses.push_back({ patternOneSynonym });
		unorderedClauses.push_back({ patternNoSynonym });

		orderedClauses.push_back({ patternNoSynonym });
		orderedClauses.push_back({ patternOneSynonym });
		orderedClauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(QueryOptimizer::optimizeClausesOrder(unorderedClauses), orderedClauses);

		unorderedClauses.clear();
		orderedClauses.clear();
		unorderedClauses.push_back({ relRefOneSynonym });
		unorderedClauses.push_back({ relRefTwoSynonym });
		unorderedClauses.push_back({ relRefNoSynonym });
		unorderedClauses.push_back({ patternTwoSynonym });
		unorderedClauses.push_back({ patternOneSynonym });
		unorderedClauses.push_back({ patternNoSynonym });

		orderedClauses.push_back({ relRefNoSynonym });
		orderedClauses.push_back({ patternNoSynonym });
		orderedClauses.push_back({ relRefOneSynonym });
		orderedClauses.push_back({ patternOneSynonym });
		orderedClauses.push_back({ relRefTwoSynonym });
		orderedClauses.push_back({ patternTwoSynonym });
		EXPECT_EQ(QueryOptimizer::optimizeClausesOrder(unorderedClauses), orderedClauses);
	}

}