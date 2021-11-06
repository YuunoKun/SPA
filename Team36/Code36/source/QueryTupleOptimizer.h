#pragma once
#include <unordered_map>
#include "Entity.h"

class QueryTupleOptimizer {
public:
	QueryTupleOptimizer();
	std::list<std::list<Entity, Entity>> extractCommonTuples(std::vector<Entity>, std::list<std::pair<Entity, Entity>>);
private:
};