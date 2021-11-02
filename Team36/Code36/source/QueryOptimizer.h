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
	bool affect_threshold_optimization = true;
	int affect_threshold_count = 10;

	std::list<Clause> optmizeClausesWithAffectThreshold(std::list<Clause>&);
	std::list<Clause> sortClausesByCommonSynonym(std::list<Clause>&);
	std::list<Clause> sortTwoSynonymClausesByCommonSynonym(std::list<Clause>&);

	std::list<Clause> sortClausesByComputationalType(std::list<Clause>&);
	std::list<Clause> sortNextTClauses(std::list<Clause>&);
	std::list<Clause> sortAffectsClauses(std::list<Clause>&);
	std::list<Clause> sortClausesByNumOfSynonym(std::list<Clause>&);

	bool isSynonymAndSynonym(Entity&, Entity&);
	bool isConstantAndConstant(Entity&, Entity&);
	bool isSynonymAndConstant(Entity&, Entity&);

	bool isNoSynonym(Entity&, Entity&);
	bool isOneSynonym(Entity&, Entity&);

	bool isDFSWhenNonTTableNotCached(Entity&, Entity&);

	bool checkConstantExist(Entity&);
	bool checkSecondaryAttributeConstantExist(Entity&);

	Entity getLeftEntity(Clause&);
	Entity getRightEntity(Clause&);
	PKBAdapter pkb;
};