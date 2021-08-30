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
			testEntity.addValue(input);
			values.insert(input);
			EXPECT_EQ(testEntity.getValues(), values);
		}

		for each (std::string input in testInput) {
			testEntity.removeValue(input);
			values.erase(input);
			EXPECT_EQ(testEntity.getValues(), values);
		}
	}
}