#include "pch.h"

#include "Entity.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Entity, getType) {
		for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
			EntityType type = static_cast<EntityType>(i);
			Entity testEntity(type, "test");
			EXPECT_EQ(testEntity.getType(), type);
		}
	}

	TEST(Entity, getName) {
		std::vector<std::string> v{ "test", "test1", "name", "whatever" };

		for each (std::string name in v) {
			Entity testEntity(STMT, name);
			EXPECT_EQ(testEntity.getName(), name);
		}
	}

	TEST(Entity, values) {
		std::vector<std::string> testInput{ "test", "test1", "test2", "test1" };
		std::unordered_set<std::string> values;

		Entity testEntity(STMT, "");

		for each (std::string input in testInput) {

			Entity testEntity(STMT, "", input);
			EXPECT_EQ(testEntity.getValue(), input);
		}
	}

	TEST(Entity, isDeclartion) {
		Entity testEntity(STMT, "test");
		EXPECT_TRUE(testEntity.isDeclaration());

		Entity testEntity2(STMT, "", "test");
		EXPECT_FALSE(testEntity2.isDeclaration());
	}
	TEST(Entity, equal) {
		Entity e1(STMT, "test");
		Entity e2(STMT, "test");
		EXPECT_EQ(e1, e2);

		Entity e3(READ, "test");
		EXPECT_FALSE(e1 == e3);

		Entity e4(STMT, "test1");
		EXPECT_FALSE(e1 == e4);

		Entity e5(WHILE, "test1");
		EXPECT_FALSE(e1 == e5);
	}
}