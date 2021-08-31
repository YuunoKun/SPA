#include "pch.h"

#include "Pattern.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Pattern, getEntityRef) {
		//for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
		//	EntityType type = static_cast<EntityType>(i);
		//	Entity testEntity(type, "test");
		//	Pattern pattern({ type, "test" }, {}, true);
		//	EXPECT_EQ(pattern.getEntityRef(), testEntity);
		//}
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
		//Entity testEntity(STMT, "test");

		//Pattern pattern1(testEntity, {}, true);
		//EXPECT_TRUE(pattern1.isWild());

		//Pattern pattern2(testEntity, {}, false);
		//EXPECT_FALSE(pattern2.isWild());
	}

	TEST(Pattern, equal) {
		//Entity testEntity(STMT, "test");

		//Pattern e1(testEntity, {}, true);
		//Pattern e2(testEntity, {}, true);
		//EXPECT_EQ(e1, e2);

		//Pattern e3(testEntity, {}, false);
		//EXPECT_FALSE(e1 == e3);

		//Entity testEntity1(STMT, "test1");
		//Pattern e4(testEntity1, {}, true);
		//EXPECT_FALSE(e1 == e4);

		//Entity testEntity2(WHILE, "test1");
		//Pattern e5(testEntity2, {}, true);
		//EXPECT_FALSE(e1 == e5);
	}
}