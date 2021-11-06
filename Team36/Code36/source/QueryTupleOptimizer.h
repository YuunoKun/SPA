#pragma once
#include <map>
#include "Entity.h"

class QueryTupleOptimizer {
public:
	QueryTupleOptimizer();
	std::list<std::list<Entity>> groupCommonEntities(std::list<std::pair<Entity, Entity>>);

private:
	std::vector<std::pair<Entity, int>> entityToInt;
	std::list<std::list<std::pair<Entity, Entity>>> result;
};