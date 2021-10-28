#pragma once
#include <list>
#include "Clause.h"
#include "PKBAdapter.h"

class QueryOptimizer {
public:
	std::vector<Clause> optimizeClausesOrder(std::vector<Clause>&);
	bool checkAllConstantExist(std::vector<Clause>&);

private:

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	bool isComputeNextT(RelType, Entity&, Entity&);
	bool isGraphSearchNextNoSynonym(RelType, Entity&, Entity&);
	bool isGraphSearchNextOneSynonym(RelType, Entity&, Entity&);


	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};