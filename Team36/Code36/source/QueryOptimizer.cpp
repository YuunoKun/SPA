#include "QueryOptimizer.h"
#include "Entity.h"
#pragma once

//Re-order the clauses in the following order
//1. clauses with no synonym
//2. clauses with one synonym
//3. clauses with two synonym
std::vector<Clause> QueryOptimizer::optimizeClausesOrder(std::vector<Clause>& clauses) {
	std::list<Clause> no_synonym_list, one_synonym_list, two_synonym_list;
	for (Clause c : clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);


		if (left_entity.isSynonym() && right_entity.isSynonym()) {
			two_synonym_list.push_back(c);
		}
		else if (left_entity.isSynonym() || right_entity.isSynonym()) {
			one_synonym_list.push_back(c);
		}
		else {
			no_synonym_list.push_back(c);
		}
	}

	std::vector<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_list.begin(), no_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), one_synonym_list.begin(), one_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_list.begin(), two_synonym_list.end());
	return reordered_clauses;
}

bool QueryOptimizer::checkAllConstantExist(std::vector<Clause>& clauses) {
	for (Clause c : clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (!checkConstantExist(left_entity) || !checkConstantExist(right_entity)) {
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
	case EntityType::PROG_LINE: return pkb.isStmt(std::stoi(e.getValue()));
	case EntityType::READ: return pkb.isRead(std::stoi(e.getValue()));
	case EntityType::PRINT: return pkb.isPrint(std::stoi(e.getValue()));
	case EntityType::CALL: return pkb.isCall(std::stoi(e.getValue()));
	case EntityType::WHILE: return pkb.isWhile(std::stoi(e.getValue()));
	case EntityType::IF: return pkb.isIf(std::stoi(e.getValue()));
	case EntityType::ASSIGN: return pkb.isAssign(std::stoi(e.getValue()));
	case EntityType::VARIABLE: return pkb.isVariable(e.getValue());
	case EntityType::CONSTANT: return pkb.isConstant(std::stoi(e.getValue()));
	case EntityType::PROCEDURE: return pkb.isProcedure(e.getValue());
	default: throw std::domain_error("Some constant has not been checked!!");
	}
	return false;
}

Entity QueryOptimizer::getLeftEntity(Clause& c) {
	switch (c.getType()) {
	case ClauseType::RELATION: return c.getRelation().getFirstClause();
	case ClauseType::PATTERN: return c.getPattern().getPatternType();
	default: throw std::domain_error("Some Clauses is not being handle!!!!");
	}
}

Entity QueryOptimizer::getRightEntity(Clause& c) {
	switch (c.getType()) {
	case ClauseType::RELATION: return c.getRelation().getSecondClause();
	case ClauseType::PATTERN: return  c.getPattern().getLeftExpression();
	default: throw std::domain_error("Some Clauses is not being handle!!!!");
	}
}
