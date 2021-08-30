#include "pch.h"

#include "Pattern.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Pattern, getEntityRef) {
		for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
			EntityType type = static_cast<EntityType>(i);
			Entity testEntity(type, "test");
			TNode tnode;
			Pattern pattern(testEntity, tnode, true);
			EXPECT_EQ(pattern.getEntityRef(), testEntity);
		}
	}

	//TODO: test case to be update with expression parser
	TEST(Pattern, getExpression) {
		/*
		Entity testEntity(STMT, "test");
		TNode tnode;
		Pattern pattern(testEntity, tnode, true);
		EXPECT_EQ(pattern.getExpression(), tnode);
		*/
	}

	TEST(Pattern, isWild) {
		Entity testEntity(STMT, "test");
		TNode tnode;

		Pattern pattern1(testEntity, tnode, true);
		EXPECT_TRUE(pattern1.isWild());

		Pattern pattern2(testEntity, tnode, false);
		EXPECT_FALSE(pattern2.isWild());
	}
}
