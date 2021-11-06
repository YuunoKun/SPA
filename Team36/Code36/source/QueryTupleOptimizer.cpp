#include "QueryTupleOptimizer.h"
#include "QueryTupleGraph.h"
#include <unordered_map>
#include "Entity.h"

QueryTupleOptimizer::QueryTupleOptimizer() {
}

std::list<std::list<std::pair<Entity, Entity>>> QueryTupleOptimizer::groupCommonEntities(std::list<std::pair<Entity, Entity>> input)
{
	int counter = 0;
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

	QueryTupleGraph g(counter);

	for (auto element : input) {
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
		g.addEdge(first_ent_no, second_ent_no);
	}

	std::list<std::list<int>> raw_result = g.connectedComponents();
	std::list<std::list<Entity>> intermediary_result;

	for (std::list<int> el : raw_result) {
		std::list<Entity> entities;
		for (int integer : el) {
			for (std::pair<Entity, int> pr : entityToInt) {
				if (pr.second == integer) {
					entities.push_back(pr.first);
				}
			}
		}
		intermediary_result.push_back(entities);
	}

	return QueryTupleOptimizer::processToEntityPairs(intermediary_result, input);
}

std::list<std::list<std::pair<Entity, Entity>>> QueryTupleOptimizer::processToEntityPairs(std::list<std::list<Entity>> entities,
	std::list<std::pair<Entity, Entity>> input) {
	std::list<std::list<std::pair<Entity, Entity>>> final_result;

	std::list<Entity>::iterator i;
	for (std::list<std::list<Entity>>::iterator i = entities.begin(); i != entities.end(); ++i) {
		std::list<std::pair<Entity, Entity>> list_of_pairs;
		for (std::list<Entity>::iterator idx = i->begin(); idx != std::prev(i->end()); ++idx) {
			std::pair<Entity, Entity> pairing1 = std::make_pair(*idx, *std::next(idx));
			std::pair<Entity, Entity> pairing2 = std::make_pair(*std::next(idx), *idx);
			for (std::pair<Entity, Entity> input_pair : input) {
				if (pairing1 == input_pair) {
					list_of_pairs.push_back(pairing1);
				}
				if (pairing2 == input_pair) {
					list_of_pairs.push_back(pairing2);
				}
			}
		}
		final_result.push_back(list_of_pairs);
	}

	return final_result;
}