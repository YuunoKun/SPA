#pragma once
#include <map>
#include "Entity.h"

class QueryTupleOptimizer {
public:
	QueryTupleOptimizer();
	std::list<std::list<std::pair<Entity, Entity>>> groupCommonEntities(std::list<std::pair<Entity, Entity>>);

private:
	std::map<Entity, int> entityToInt;
};