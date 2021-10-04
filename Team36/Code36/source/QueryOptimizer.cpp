#include "QueryOptimizer.h"
#pragma once

//Re-order the clauses in the following order
//1. clauses with no synonym
//2. clauses with one synonym
//3. clauses with two synonym
std::vector<Clause> QueryOptimizer::optimizeClausesOrder(std::vector<Clause>& clauses) {
	std::list<Clause> noSynonymList, oneSynonymList, twoSynonymList;
	for (Clause c : clauses) {
		Entity leftEntity;
		Entity rightEntity;

		switch (c.getType()) {
		case RELATION:
			leftEntity = c.getRelation().getFirstClause();
			rightEntity = c.getRelation().getSecondClause();
			break;
		case PATTERN:
			leftEntity = c.getPattern().getPatternType();
			rightEntity = c.getPattern().getLeftExpression();
			break;
		default: throw std::domain_error("Some Clauses is not being handle!!!!");
		}

		if (leftEntity.isSynonym() && rightEntity.isSynonym()) {
			twoSynonymList.push_back(c);
		}else if (leftEntity.isSynonym() || rightEntity.isSynonym()) {
			oneSynonymList.push_back(c);
		}else {
			noSynonymList.push_back(c);
		}
	}

	std::vector<Clause> reorderedClauses;
	reorderedClauses.insert(reorderedClauses.end(), noSynonymList.begin(), noSynonymList.end());
	reorderedClauses.insert(reorderedClauses.end(), oneSynonymList.begin(), oneSynonymList.end());
	reorderedClauses.insert(reorderedClauses.end(), twoSynonymList.begin(), twoSynonymList.end());
	return reorderedClauses;
}