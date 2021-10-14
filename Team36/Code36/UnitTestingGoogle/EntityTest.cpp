#include "pch.h"

#include "Entity.h"
#include "Common.h"

namespace UnitTesting {
	TEST(Entity, getType) {
		for (int i = EntityType::STMT; i != EntityType::WILD; ++i) {
			EntityType type = static_cast<EntityType>(i);
			Entity test_entity(type, Synonym{ "test" });
			EXPECT_EQ(test_entity.getType(), type);
		}
	}

	TEST(Entity, getName) {
		std::vector<Synonym> v{ {"test"}, {"test1"}, {"name"}, {"whatever"} };

		for each (Synonym name in v) {
			Entity test_entity(STMT, name);
			EXPECT_EQ(test_entity.getSynonym(), name.name);
		}
	}

	TEST(Entity, values) {
		std::vector<std::string> testInput{ "test", "test1", "test2", "test1" };
		std::unordered_set<std::string> values;

		Entity test_entity(STMT, "");

		for each (std::string input in testInput) {
			Entity test_entity(STMT, input);
			EXPECT_EQ(test_entity.getValue(), input);
		}
	}

	TEST(Entity, attribute) {
		std::vector<Entity> e;
		std::vector<AttrRef> r;
		e.push_back({ STMT, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ READ, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ PRINT, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ CALL, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ WHILE, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ IF, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ ASSIGN, Synonym("a") });
		r.push_back(STMT_INDEX);
		e.push_back({ VARIABLE, Synonym("a") });
		r.push_back(VAR_NAME);
		e.push_back({ PROCEDURE, Synonym("a") });
		r.push_back(PROC_NAME);
		e.push_back({ CONSTANT, Synonym("a") });
		r.push_back(VALUE);

		e.push_back({ READ, Synonym("a"), VAR_NAME });
		r.push_back(VAR_NAME);
		e.push_back({ PRINT, Synonym("a"), VAR_NAME });
		r.push_back(VAR_NAME);
		e.push_back({ CALL, Synonym("a"), PROC_NAME });
		r.push_back(PROC_NAME);

		for (int i = 0; i < e.size(); i++) {
			EXPECT_EQ(e[i].getAttribute(), r[i]);
		}
	}

	TEST(Entity, isDeclartion) {
		Synonym s = { "test" };
		Entity test_entity(STMT, s);
		EXPECT_TRUE(test_entity.isSynonym());

		Entity test_entity2(STMT, "test");
		EXPECT_FALSE(test_entity2.isSynonym());
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