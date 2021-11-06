#include "QueryTupleOptimizer.h"
#include <unordered_map>
#include "Entity.h"

QueryTupleOptimizer::QueryTupleOptimizer() {
}

std::list<std::list<std::pair<Entity, Entity>>> groupCommonEntities(std::list<std::pair<Entity, Entity>> input) {
	for (auto element : input) {
		Entity first_ent = element.first;
		Entity second_ent = element.second;
	}
}