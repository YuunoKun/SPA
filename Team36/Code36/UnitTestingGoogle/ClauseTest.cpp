#include "pch.h"

#include "Clause.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Clause, PatternClause) {
		Pattern pattern({ ASSIGN, Synonym{ "a" } }, { VARIABLE, Synonym{ "v" } }, "", true);
		Clause clause(pattern);
		EXPECT_EQ(clause, Clause(pattern));
		EXPECT_EQ(clause.getPattern(), pattern);
		EXPECT_EQ(clause.getPattern().getPatternType(), pattern.getPatternType());
		EXPECT_EQ(clause.getPattern().getLeftExpression(), pattern.getLeftExpression());
		EXPECT_EQ(clause.getPattern().getExpression(), pattern.getExpression());
		EXPECT_EQ(clause.getPattern().isWild(), pattern.isWild());

		EXPECT_EQ(clause.getPattern().getPatternType(), Entity( ASSIGN, Synonym{ "a" } ) );
		EXPECT_EQ(clause.getPattern().getLeftExpression(), Entity(VARIABLE, Synonym{ "v" }));
		EXPECT_EQ(clause.getPattern().getExpression(), nullptr);
		EXPECT_EQ(clause.getPattern().isWild(), true);


		EXPECT_EQ(pattern.getPatternType(), Entity(ASSIGN, Synonym{ "a" }));
		EXPECT_EQ(pattern.getLeftExpression(), Entity(VARIABLE, Synonym{ "v" }));
		EXPECT_EQ(pattern.getExpression(), nullptr);
		EXPECT_EQ(pattern.isWild(), true);

		EXPECT_EQ(clause.getType(), PATTERN);
	}

	TEST(Clause, RelationClause) {
		RelRef relation(MODIFIES_S, { ASSIGN, Synonym{ "a" } }, { VARIABLE, Synonym{ "v" } });
		Clause clause(relation);
		EXPECT_EQ(clause.getRelation(), relation);
		EXPECT_EQ(clause.getType(), RELATION);
	}
}