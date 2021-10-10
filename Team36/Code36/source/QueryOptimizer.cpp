#include "QueryOptimizer.h"
#include "Entity.h"
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
		case ClauseType::RELATION:
			leftEntity = c.getRelation().getFirstClause();
			rightEntity = c.getRelation().getSecondClause();
			break;
		case ClauseType::PATTERN:
			leftEntity = c.getPattern().getPatternType();
			rightEntity = c.getPattern().getLeftExpression();
			break;
		default: throw std::domain_error("Some Clauses is not being handle!!!!");
		}

		if (leftEntity.isSynonym() && rightEntity.isSynonym()) {
			twoSynonymList.push_back(c);
		}
		else if (leftEntity.isSynonym() || rightEntity.isSynonym()) {
			oneSynonymList.push_back(c);
		}
		else {
			noSynonymList.push_back(c);
		}
	}

	std::vector<Clause> reorderedClauses;
	reorderedClauses.insert(reorderedClauses.end(), noSynonymList.begin(), noSynonymList.end());
	reorderedClauses.insert(reorderedClauses.end(), oneSynonymList.begin(), oneSynonymList.end());
	reorderedClauses.insert(reorderedClauses.end(), twoSynonymList.begin(), twoSynonymList.end());
	return reorderedClauses;
}

bool QueryOptimizer::checkAllConstantExist(std::vector<Clause>& clauses) {
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
		if (!checkConstantExist(leftEntity) || !checkConstantExist(rightEntity)) {
			return false;
		}
	}

	return true;
}

bool QueryOptimizer::checkConstantExist(Entity& e) {
	if (e.isSynonym() || e.getType() == EntityType::WILD) {
		return true;
	}
	switch (e.getType()) {
	case EntityType::STMT:
	case EntityType::PROG_LINE: return pkb.isStmt(std::stoi(e.getValue())); break;
	case EntityType::READ: return pkb.isRead(std::stoi(e.getValue())); break;
	case EntityType::PRINT: return pkb.isPrint(std::stoi(e.getValue())); break;
	case EntityType::CALL: return pkb.isCall(std::stoi(e.getValue())); break;
	case EntityType::WHILE: return pkb.isWhile(std::stoi(e.getValue())); break;
	case EntityType::IF: return pkb.isIf(std::stoi(e.getValue())); break;
	case EntityType::ASSIGN: return pkb.isAssign(std::stoi(e.getValue())); break;
	case EntityType::VARIABLE: return pkb.isVariable(e.getValue()); break;
	case EntityType::CONSTANT: return pkb.isConstant(std::stoi(e.getValue())); break;
	case EntityType::PROCEDURE: return pkb.isProcedure(e.getValue()); break;
	default: throw std::domain_error("Some constant has not been checked!!");
	}
	return false;
}