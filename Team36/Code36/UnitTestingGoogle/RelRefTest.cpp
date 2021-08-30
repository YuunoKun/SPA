#include "pch.h"

#include "RelRef.h"
#include "Common.h"

namespace UnitTesting {
	TEST(RelRef, getType) {
		for (int i = RelType::MODIFIES_S; i != RelType::FOLLOWS_T; ++i) {
			RelType type = static_cast<RelType>(i);
			RelRef rel(type, {});
			EXPECT_EQ(rel.getType(), type);
		}
	}

	TEST(RelRef, getClauses) {
		std::vector<Entity> clauses{ {STMT,"test1"},{READ, "test2"} };


		RelRef rel(MODIFIES_S, clauses);
		EXPECT_EQ(rel.getClauses(), clauses);
	}

	TEST(RelRef, equal) {
		RelRef e1(MODIFIES_S, { { STMT,"test1" }, { READ, "test2" } });
		RelRef e2(MODIFIES_S, { { STMT,"test1" }, { READ, "test2" } });
		EXPECT_EQ(e1, e2);

		RelRef e3(MODIFIES_P, { { STMT,"test1" }, { READ, "test2" } });
		EXPECT_FALSE(e1 == e3);

		RelRef e4(MODIFIES_P, { { STMT,"test1" } });
		EXPECT_FALSE(e1 == e4);

		RelRef e5(FOLLOWS, { { STMT,"test1" } });
		EXPECT_FALSE(e1 == e5);
	}
}