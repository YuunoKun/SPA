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
}