#include "pch.h"

#include "Entity.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Entity, getType) {
		for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
			EntityType type = static_cast<EntityType>(i);
			Entity testEntity(type, Synonym{ "test" });
			EXPECT_EQ(testEntity.getType(), type);
		}
	}

	TEST(Entity, getName) {
		std::vector<Synonym> v{ {"test"}, {"test1"}, {"name"}, {"whatever"} };

		for each (Synonym name in v) {
			Entity testEntity(STMT, name);
			EXPECT_EQ(testEntity.getSynonym(), name.name);
		}
	}

	TEST(Entity, values) {
		std::vector<std::string> testInput{ "test", "test1", "test2", "test1" };
		std::unordered_set<std::string> values;

		Entity testEntity(STMT, "");

		for each (std::string input in testInput) {
			Entity testEntity(STMT, input);
			EXPECT_EQ(testEntity.getValue(), input);
		}
	}

	TEST(Entity, isDeclartion) {
		Synonym s = { "test" };
		Entity testEntity(STMT, s);
		EXPECT_TRUE(testEntity.isSynonym());

		Entity testEntity2(STMT, "test");
		EXPECT_FALSE(testEntity2.isSynonym());
	}
	TEST(Entity, equal) {
		Entity e1(STMT, Synonym{ "test" });
		Entity e2(STMT, Synonym{ "test" });
		EXPECT_EQ(e1, e2);

		Entity e3(READ, Synonym{ "test" });
		EXPECT_FALSE(e1 == e3);

		Entity e4(STMT, Synonym{ "test1" });
		EXPECT_FALSE(e1 == e4);

		Entity e5(WHILE, Synonym{ "test1" });
		EXPECT_FALSE(e1 == e5);

		Entity e6(STMT, "test");
		Entity e7(STMT, "test");
		EXPECT_TRUE(e6 == e7);
		EXPECT_FALSE(e1 == e6);
	}
}