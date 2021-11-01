#pragma once
#include <list>
#include "Clause.h"
#include "PKBAdapter.h"

class QueryOptimizer {
public:
	QueryOptimizer();
	QueryOptimizer(bool, bool);
	QueryOptimizer(bool, bool, bool, int);

	std::vector<Clause> optimizeClausesOrder(std::vector<Clause>&);
	bool checkAllConstantExist(std::vector<Clause>&);

private:
	bool optimize_clause_by_common_synonym = true;
	bool optimize_clause_by_relation_type = true;
	bool affect_threshold_optimization = false;
	int affect_threshold_count = 0;

	std::list<Clause> optimizeClausesOrderByCommonSynonym(std::list<Clause>&);
	std::list<Clause> optimizeTwoSynonymClausesOrder(std::list<Clause>&);

	std::list<Clause> optimizeClausesOrderByRelationType(std::list<Clause>&);
	std::list<Clause> optimizeNextTClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeAffectsClausesOrder(std::list<Clause>&);
	std::list<Clause> optimizeClausesByNumOfSynonym(std::list<Clause>&);

	bool isSynonymAndSynonym(Entity&, Entity&);
	bool isConstantAndConstant(Entity&, Entity&);
	bool isSynonymAndConstant(Entity&, Entity&);


	bool isNoSynonym(Entity&, Entity&);
	bool isOneSynonym(Entity&, Entity&);

	bool isCFGSearchWithNonTCache(Entity&, Entity&);

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};