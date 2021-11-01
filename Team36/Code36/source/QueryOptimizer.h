#pragma once
#include <list>
#include "Clause.h"
#include "PKBAdapter.h"

class QueryOptimizer {
public:
	std::vector<Clause> optimizeClausesOrder(std::vector<Clause>&);
	bool checkAllConstantExist(std::vector<Clause>&);

private:
	bool optimize_clause_by_common_synonym = true;
	bool optimize_clause_by_relation_type = true;

	std::vector<Clause> optimizeClausesOrderByCommonSynonym(std::vector<Clause>&);
	std::list<Clause> optimizeTwoSynonymClausesOrder(std::list<Clause>&);

	std::vector<Clause> optimizeClausesOrderByRelationType(std::vector<Clause>&);
	std::list<Clause> optimizeNextClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeAffectClausesOrder(std::list<Clause>&);

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	bool isSynonymAndSynonym(Entity&, Entity&);
	bool isConstantAndConstant(Entity&, Entity&);
	bool isSynonymAndConstant(Entity&, Entity&);


	bool isNoSynonym(Entity&, Entity&);
	bool isOneSynonym(Entity&, Entity&);

	bool isNextTWithCFGSearch(Entity&, Entity&);


	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};