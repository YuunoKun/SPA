#pragma once
#include <list>
#include "Clause.h"
#include "PKBAdapter.h"

class QueryOptimizer {
public:
	std::vector<Clause> optimizeClausesOrder(std::vector<Clause>&);
	bool checkAllConstantExist(std::vector<Clause>&);

private:

	std::list<Clause> optimizeNextClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeAffectClausesOrder(std::list<Clause>&);

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	bool isComputeAndCache(Entity&, Entity&);
	bool isCFGSearchNoSynonym(Entity&, Entity&);
	bool isCFGSearchOneSynonym(Entity&, Entity&);

	bool isNextTWithCFGSearch(Entity&, Entity&);


	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};