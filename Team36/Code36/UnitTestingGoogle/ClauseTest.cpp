#include "pch.h"

#include "Clause.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Clause, PatternClause) {
		Pattern pattern({ ASSIGN, Synonym{ "a" } }, { VARIABLE, Synonym{ "v" } }, "", true);
		Clause clause(pattern);
		EXPECT_EQ(clause.getPattern(), pattern);
		EXPECT_EQ(clause.getType(), PATTERN);
	}

	TEST(Clause, RelationClause) {
		RelRef relation(MODIFIES_S, { ASSIGN, Synonym{ "a" } }, { VARIABLE, Synonym{ "v" } });
		Clause clause(relation);
		EXPECT_EQ(clause.getRelation(), relation);
		EXPECT_EQ(clause.getType(), RELATION);
	}
}