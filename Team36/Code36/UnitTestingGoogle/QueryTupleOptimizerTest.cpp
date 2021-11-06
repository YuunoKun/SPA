#include "pch.h"

#include "QueryTupleOptimizer.h"

namespace UnitTesting {
	TEST(QueryTupleOptimizer, allSameGroup) {
		QueryTupleOptimizer qto;
		std::list<std::pair<Entity, Entity>> input;
		Entity ent1 = Entity(EntityType::STMT, Synonym{ "a" });
		Entity ent2 = Entity(EntityType::STMT, Synonym{ "b" });
		Entity ent3 = Entity(EntityType::STMT, Synonym{ "c" });
		Entity ent4 = Entity(EntityType::STMT, Synonym{ "d" });
		input.push_back(std::make_pair(ent1, ent2));
		input.push_back(std::make_pair(ent2, ent3));
		input.push_back(std::make_pair(ent3, ent4));

		std::list<std::list<Entity>> res;
		res = qto.groupCommonEntities(input);

		std::list<std::list<Entity>> expected_output;
		std::list<Entity> list_output;
		list_output.push_back(ent1);
		list_output.push_back(ent2);
		list_output.push_back(ent3);
		list_output.push_back(ent4);
		expected_output.push_back(list_output);

		EXPECT_EQ(res, expected_output);
	}

	TEST(QueryTupleOptimizer, twoSeparateGroups) {
		QueryTupleOptimizer qto;
		std::list<std::pair<Entity, Entity>> input;
		Entity ent1 = Entity(EntityType::STMT, Synonym{ "a" });
		Entity ent2 = Entity(EntityType::STMT, Synonym{ "b" });
		Entity ent3 = Entity(EntityType::STMT, Synonym{ "c" });
		Entity ent4 = Entity(EntityType::STMT, Synonym{ "d" });
		input.push_back(std::make_pair(ent1, ent2));
		input.push_back(std::make_pair(ent3, ent4));

		std::list<std::list<Entity>> res;
		res = qto.groupCommonEntities(input);

		std::list<std::list<Entity>> expected_output;
		std::list<Entity> list_output;
		list_output.push_back(ent1);
		list_output.push_back(ent2);

		std::list<Entity> list_output2;
		list_output2.push_back(ent3);
		list_output2.push_back(ent4);

		expected_output.push_back(list_output);
		expected_output.push_back(list_output2);

		EXPECT_EQ(res, expected_output);
	}
}