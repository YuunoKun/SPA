#include "QueryTupleOptimizer.h"
#include "QueryTupleGraph.h"
#include <unordered_map>
#include "Entity.h"

QueryTupleOptimizer::QueryTupleOptimizer() {
}

std::list<std::list<std::pair<Entity, Entity>>> QueryTupleOptimizer::groupCommonEntities(std::list<std::pair<Entity, Entity>> input)
{
	int counter = 0;
	QueryTupleOptimizer::mapEntityToInt(input, counter);

	QueryTupleGraph g(counter);

	for (auto element : input) {
		std::pair<int, int> pair_numbers = QueryTupleOptimizer::mapEntity(element);
		g.addEdge(pair_numbers.first, pair_numbers.second);
	}

	std::list<std::list<int>> raw_result = g.connectedComponents();
	std::list<std::list<Entity>> intermediary_result;

	for (std::list<int> el : raw_result) {
		std::list<Entity> entities;
		for (int integer : el) {
			QueryTupleOptimizer::addEntityToInt(integer, entities);
		}
		intermediary_result.push_back(entities);
	}

	return QueryTupleOptimizer::processToEntityPairs(intermediary_result, input);
}

std::list<std::list<std::pair<Entity, Entity>>> QueryTupleOptimizer::processToEntityPairs(std::list<std::list<Entity>> entities,
	std::list<std::pair<Entity, Entity>> input) {
	std::list<std::list<std::pair<Entity, Entity>>> final_result;
	for (std::list<Entity> list_of_entities : entities) {
		std::list<std::pair<Entity, Entity>> list_of_pairs;
		for (std::pair<Entity, Entity> input_pair : input) {
			for (Entity ent : list_of_entities) {
				if (input_pair.first == ent) {
					list_of_pairs.push_back(input_pair);
				}
			}
		}

		final_result.push_back(list_of_pairs);
	}

	return final_result;
}

void QueryTupleOptimizer::mapEntityToInt(std::list<std::pair<Entity, Entity>>& input, int& counter) {
	for (std::pair<Entity, Entity> element : input) {
		Entity first_ent = element.first;
		Entity second_ent = element.second;
		bool is_first_exist = false;
		for (std::pair<Entity, int> pr : entityToInt) {
			if (pr.first == first_ent) {
				is_first_exist = true;
			}
		}

		if (!is_first_exist) {
			entityToInt.push_back(std::make_pair(first_ent, counter));
			counter++;
		}

		bool is_second_exist = false;
		for (std::pair<Entity, int> pr : entityToInt) {
			if (pr.first == second_ent) {
				is_second_exist = true;
			}
		}

		if (!is_second_exist) {
			entityToInt.push_back(std::make_pair(second_ent, counter));
			counter++;
		}
	}
}

std::pair<int, int> QueryTupleOptimizer::mapEntity(std::pair<Entity, Entity> element) {
	Entity first_ent = element.first;
	Entity second_ent = element.second;
	int first_ent_no = -1;
	int second_ent_no = -1;
	for (std::pair<Entity, int> pr : entityToInt) {
		if (pr.first == first_ent) {
			first_ent_no = pr.second;
		}
		if (pr.first == second_ent) {
			second_ent_no = pr.second;
		}
	}
	return std::pair(std::make_pair(first_ent_no, second_ent_no));
}

void QueryTupleOptimizer::addEntityToInt(int integer, std::list<Entity>& entities) {
	for (std::pair<Entity, int> pr : entityToInt) {
		if (pr.second == integer) {
			entities.push_back(pr.first);
		}
	}
}