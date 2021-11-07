#pragma once
#include <map>
#include "Entity.h"

class QueryTupleOptimizer {
public:
	QueryTupleOptimizer();
	std::list<std::list<std::pair<Entity, Entity>>> groupCommonEntities(std::list<std::pair<Entity, Entity>>);

private:
	std::vector<std::pair<Entity, int>> entityToInt;
	std::list<std::list<std::pair<Entity, Entity>>> processToEntityPairs(std::list<std::list<Entity>>, std::list<std::pair<Entity, Entity>> input);
	void mapEntityToInt(std::list<std::pair<Entity, Entity>>&, int&);
	std::pair<int, int> mapEntity(std::pair<Entity, Entity>);
	void addEntityToInt(int, std::list<Entity>&);
	void addListPairs(std::pair<Entity, Entity>&, std::list<std::pair<Entity, Entity>>&, std::list<Entity>&);
	void addNonExistingEntity(Entity&, int&);
};