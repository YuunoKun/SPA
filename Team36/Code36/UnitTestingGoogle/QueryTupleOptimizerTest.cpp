#include "pch.h"

#include "QueryTupleOptimizer.h"
#include <iostream>

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

		std::list<std::list<std::pair<Entity, Entity>>> res;
		res = qto.groupCommonEntities(input);

		std::list<std::list<std::pair<Entity, Entity>>> expected_output;
		std::list<std::pair<Entity, Entity>> list_pair_output;
		std::pair<Entity, Entity> pair_output1;
		std::pair<Entity, Entity> pair_output2;
		std::pair<Entity, Entity> pair_output3;
		pair_output1 = std::make_pair(ent1, ent2);
		pair_output2 = std::make_pair(ent2, ent3);
		pair_output3 = std::make_pair(ent3, ent4);
		list_pair_output.push_back(pair_output1);
		list_pair_output.push_back(pair_output2);
		list_pair_output.push_back(pair_output3);
		expected_output.push_back(list_pair_output);

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

		std::list<std::list<std::pair<Entity, Entity>>> res;
		res = qto.groupCommonEntities(input);

		std::list<std::list<std::pair<Entity, Entity>>> expected_output;
		std::list<std::pair<Entity, Entity>> list_pair_output1;
		std::list<std::pair<Entity, Entity>> list_pair_output2;
		std::pair<Entity, Entity> pair_output1;
		std::pair<Entity, Entity> pair_output2;
		pair_output1 = std::make_pair(ent1, ent2);
		pair_output2 = std::make_pair(ent3, ent4);
		list_pair_output1.push_back(pair_output1);
		list_pair_output2.push_back(pair_output2);
		expected_output.push_back(list_pair_output1);
		expected_output.push_back(list_pair_output2);

		EXPECT_EQ(res, expected_output);
	}
}