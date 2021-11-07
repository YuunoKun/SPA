#include "pch.h"

#include "RelRef.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelRef, getType) {
		Entity entity1(PROCEDURE, Synonym{ "test" });
		Entity entity2(VARIABLE, Synonym{ "test1" });
		RelRef valid_relref(USES_P, entity1, entity2);

		EXPECT_EQ(valid_relref.getType(), USES_P);
		EXPECT_NE(valid_relref.getType(), MODIFIES_P);
	}

	TEST(RelRef, getClauses) {
		Entity entity1(PROCEDURE, Synonym{ "test" });
		Entity entity2(VARIABLE, Synonym{ "test1" });
		RelRef valid_relref(USES_P, entity1, entity2);

		EXPECT_EQ(valid_relref.getFirstClause(), entity1);
		EXPECT_EQ(valid_relref.getSecondClause(), entity2);
	}

	TEST(RelRef, equal) {
		RelRef e1(USES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{ "test2" } });
		RelRef e2(USES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test2"} });
		EXPECT_EQ(e1, e2);

		RelRef e3(MODIFIES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test2"} });
		EXPECT_FALSE(e1 == e3);

		RelRef e4(MODIFIES_P, { PROCEDURE, Synonym{"test1"} }, { VARIABLE, Synonym{"test11"} });
		EXPECT_FALSE(e1 == e4);

		RelRef e5(FOLLOWS, { WHILE, Synonym{"test1"} }, { WHILE, Synonym{"test13"} });
		EXPECT_FALSE(e1 == e5);
	}
}