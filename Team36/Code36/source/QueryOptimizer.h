#pragma once
#include <list>
#include "Clause.h"
#include "PKBAdapter.h"

class QueryOptimizer {
public:
	std::vector<Clause> optimizeClausesOrderByCommonSynonym(std::vector<Clause>&);
	std::vector<Clause> optimizeClausesOrderByRelationType(std::vector<Clause>&);
	bool checkAllConstantExist(std::vector<Clause>&);

private:

	std::list<Clause> optimizeTwoSynonymClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeNextClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeAffectClausesOrder(std::list<Clause>&);

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	bool isSynonymAndSynonym(Entity&, Entity&);
	bool isConstantAndConstant(Entity&, Entity&);
	bool isSynonymAndWild(Entity&, Entity&);

	bool isNextTWithCFGSearch(Entity&, Entity&);


	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};