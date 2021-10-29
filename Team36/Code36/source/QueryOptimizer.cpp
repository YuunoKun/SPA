#include "QueryOptimizer.h"
#include "Entity.h"
#include "Utility.h"

//Re-order the clauses in the following order
//1. clauses with no synonym
//2. clauses with one synonym
//3. clauses with two synonym
std::vector<Clause> QueryOptimizer::optimizeClausesOrder(std::vector<Clause>& clauses) {
	std::list<Clause> no_synonym_list, one_synonym_list, two_synonym_list, compute_next_graph, 
		graph_search_next_t_no_synonym_list, graph_search_next_t_one_synonym_list, next_t_two_synonym_list_after_compute;
	for (Clause c : clauses) {
		ClauseType type = c.getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (type == ClauseType::RELATION && isComputeNextT(c.getRelation().getType(), left_entity, right_entity)) {
			if (compute_next_graph.empty()) {
				compute_next_graph.emplace_back(c);
			} else {
				next_t_two_synonym_list_after_compute.emplace_back(c);
			}

		}
		else if (type == ClauseType::RELATION && isGraphSearchNextNoSynonym(c.getRelation().getType(), left_entity, right_entity)) {
			graph_search_next_t_no_synonym_list.emplace_back(c);
		}
		else if (type == ClauseType::RELATION && isGraphSearchNextOneSynonym(c.getRelation().getType(), left_entity, right_entity)) {
			graph_search_next_t_one_synonym_list.emplace_back(c);
		}
		else if (left_entity.isSynonym() && right_entity.isSynonym()) {
			two_synonym_list.emplace_back(c);
		}
		else if (left_entity.isSynonym() || right_entity.isSynonym()) {
			one_synonym_list.emplace_back(c);
		}
		else {
			no_synonym_list.emplace_back(c);
		}
	}

	std::vector<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_list.begin(), no_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), one_synonym_list.begin(), one_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_list.begin(), two_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), compute_next_graph.begin(), compute_next_graph.end());
	reordered_clauses.insert(reordered_clauses.end(), graph_search_next_t_no_synonym_list.begin(), graph_search_next_t_no_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), graph_search_next_t_one_synonym_list.begin(), graph_search_next_t_one_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), next_t_two_synonym_list_after_compute.begin(), next_t_two_synonym_list_after_compute.end());
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
	if (Utility::isSecondaryAttribute(e)) {
		return checkSecondaryAttributeConstantExist(e);
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

bool QueryOptimizer::checkSecondaryAttributeConstantExist(Entity& e) {

	switch (e.getType()) {
	case EntityType::READ:
		if (e.getAttribute() == AttrRef::VAR_NAME) {
			return pkb.isReadVar(e.getValue());
		}
		break;
	case EntityType::PRINT:
		if (e.getAttribute() == AttrRef::VAR_NAME) {
			return pkb.isPrintVar(e.getValue());
		}
		break;
	case EntityType::CALL:
		if (e.getAttribute() == AttrRef::PROC_NAME) {
			return pkb.isCalledS(e.getValue());
		}
		break;
	}

	throw std::domain_error("checkSecondaryAttributeConstantExist(Entity): Entity type does not have Secondary Attribute!");
	return false;
}

bool QueryOptimizer::isComputeNextT(RelType type, Entity& e1, Entity& e2) {
	return type == RelType::NEXT_T && e1.isSynonym() && e2.isSynonym();
}

bool QueryOptimizer::isGraphSearchNextNoSynonym(RelType type, Entity& e1, Entity& e2) {
	return type == RelType::NEXT_T && (!e1.isSynonym() && e1.getType() != EntityType::WILD) &&
		(!e2.isSynonym() && e2.getType() != EntityType::WILD);
}

bool QueryOptimizer::isGraphSearchNextOneSynonym(RelType type, Entity& e1, Entity& e2) {
	return type == RelType::NEXT_T && 
		((e1.isSynonym() && (!e2.isSynonym() && e2.getType() != EntityType::WILD)) ||
		(e2.isSynonym() && (!e1.isSynonym() && e1.getType() != EntityType::WILD)));
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
