#include "QueryOptimizer.h"
#include "Entity.h"
#include "Utility.h"



std::vector<Clause> QueryOptimizer::optimizeClausesOrder(std::vector<Clause>& clauses) {
	if (optimize_clause_by_common_synonym) {
		clauses = optimizeClausesOrderByCommonSynonym(clauses);
	}
	
	if (optimize_clause_by_relation_type) {
		clauses = optimizeClausesOrderByRelationType(clauses);
	}
	return clauses;
}


//Re-order the clauses in the following order
//1. clauses with no synonym
//2. clauses with one synonym
//3. clauses with two synonym
std::vector<Clause> QueryOptimizer::optimizeClausesOrderByCommonSynonym(std::vector<Clause>& clauses) {
	std::list<Clause> no_synonym_list, one_synonym_list, two_synonym_list;
	for (Clause c : clauses) {
		ClauseType type = c.getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (left_entity.isSynonym() && right_entity.isSynonym()) {
			two_synonym_list.emplace_back(c);
		}
		else if (left_entity.isSynonym() || right_entity.isSynonym()) {
			one_synonym_list.emplace_back(c);
		}
		else {
			no_synonym_list.emplace_back(c);
		}
	}

	two_synonym_list = optimizeTwoSynonymClausesOrder(two_synonym_list);
	std::vector<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_list.begin(), no_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), one_synonym_list.begin(), one_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_list.begin(), two_synonym_list.end());
	return reordered_clauses;
}


//Re-order the clauses in the following order to delay joining of table 
//1. clauses with no common synonym
//2. clauses with two common synonym
//3. clauses with one common synonym
std::list<Clause> QueryOptimizer::optimizeTwoSynonymClausesOrder(std::list<Clause>& two_synonym_clauses) {
	return two_synonym_clauses;
}


//Re-order the clauses in the following order according to their computational intencity in increasing other.
//1. other clauses
//2. next clauses
//3. affect clauses
std::vector<Clause> QueryOptimizer::optimizeClausesOrderByRelationType(std::vector<Clause>& clauses) {
	std::list<Clause> others_clauses, next_t_cfg_search_clauses, affects_clauses;
	for (Clause c : clauses) {
		ClauseType type = c.getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (type == ClauseType::RELATION && c.getRelation().getType() == NEXT_T
			&& isNextTWithCFGSearch(left_entity, right_entity)) {
			next_t_cfg_search_clauses.emplace_back(c);
		}
		else if (type == ClauseType::RELATION &&
			(c.getRelation().getType() == AFFECT || c.getRelation().getType() == AFFECT_T)) {
			affects_clauses.emplace_back(c);
		}
		else {
			others_clauses.emplace_back(c);
		}
	}
	next_t_cfg_search_clauses = optimizeNextClausesOrder(next_t_cfg_search_clauses);
	affects_clauses = optimizeAffectClausesOrder(affects_clauses);

	std::vector<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), others_clauses.begin(), others_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), next_t_cfg_search_clauses.begin(), next_t_cfg_search_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affects_clauses.begin(), affects_clauses.end());
	return reordered_clauses;
}


//Re-order the clauses in the following order
//1. first two common synonym next clauses (to cache the result)
//2. Next_T clauses that require graph search and terminate whenever a single result found
//3. Next_T clauses that require graph search and terminate when all relevant result is found
//2. remaining two synonym clauses
std::list<Clause> QueryOptimizer::optimizeNextClausesOrder(std::list<Clause>& next_t_clauses) {
	std::list<Clause> compute_next_graph, no_synonym_clauses, 
		one_synonym_clauses, two_synonym_clauses_after_pre_compute;

	for (Clause c : next_t_clauses) {
		ClauseType type = c.getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity)) {
			if (compute_next_graph.empty()) {
				compute_next_graph.emplace_back(c);
			}
			else {
				two_synonym_clauses_after_pre_compute.emplace_back(c);
			}

		}
		else if (isConstantAndConstant(left_entity, right_entity)) {
			no_synonym_clauses.emplace_back(c);
		}
		else if (isSynonymAndConstant(left_entity, right_entity)) {
			one_synonym_clauses.emplace_back(c);
		}
		else {
			throw std::domain_error("optimizeNextClausesOrder() :: Some next clauses has not been handle!");
		}

	}

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), compute_next_graph.begin(), compute_next_graph.end());
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_clauses.begin(), no_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), one_synonym_clauses.begin(), one_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_clauses_after_pre_compute.begin(), two_synonym_clauses_after_pre_compute.end());
	return reordered_clauses;
}


//Re-order the clauses in the following order
//1. first two common synonym affect* or affect clauses (to cache the result)
//2. Affects/Affect_T clauses that require graph search and terminate whenever single result found
//4. Affects clauses that require graph search and terminate when all relevant result is found or with specfic target e.g (Affect*(1,30))
//5. Remaining two synonym affects clauses
//7. Affects_T clauses that require graph search and terminate when all relevant result is found or with specfic target e.g (Affect*(1,30))
//8. Remaining two synonym affects_T clauses
std::list<Clause> QueryOptimizer::optimizeAffectClausesOrder(std::list<Clause>& affect_clauses) {

	std::list<Clause> compute_affect_graph, no_synonym_clauses,
		affect_one_synonym_clauses, affect_two_synonym_clauses,
		affect_t_one_synonym_clauses, affect_t_two_synonym_clauses;

	for (Clause c : affect_clauses) {
		ClauseType type = c.getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity)) {
			if (compute_affect_graph.empty()) {
				compute_affect_graph.emplace_back(c);
			}
			else if(compute_affect_graph.front().getType() == RelType::AFFECT && c.getRelation().getType() == RelType::AFFECT_T){
				compute_affect_graph.emplace_back(c);
				affect_two_synonym_clauses.emplace_back(compute_affect_graph.front());
				compute_affect_graph.pop_front();
			}
			else if (c.getRelation().getType() == RelType::AFFECT_T) {
				affect_t_two_synonym_clauses.emplace_back(c);
			}
			else {
				affect_two_synonym_clauses.emplace_back(c);
			}

		}else if (isConstantAndConstant(left_entity, right_entity) || isOneSynonym(left_entity, right_entity)) {
			if (c.getRelation().getType() == RelType::AFFECT_T) {
				affect_one_synonym_clauses.emplace_back(c);
			}
			else {
				affect_t_one_synonym_clauses.emplace_back(c);
			}
		}
		else if (isNoSynonym(left_entity, right_entity)) {
			no_synonym_clauses.emplace_back(c);
		}
		else {
			throw std::domain_error("optimizeAffectClausesOrder() :: Some affects clauses has not been handle!");
		}

	}

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), compute_affect_graph.begin(), compute_affect_graph.end());
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_clauses.begin(), no_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_one_synonym_clauses.begin(), affect_one_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_two_synonym_clauses.begin(), affect_two_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_t_one_synonym_clauses.begin(), affect_t_one_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_t_two_synonym_clauses.begin(), affect_t_two_synonym_clauses.end());
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

bool QueryOptimizer::isSynonymAndSynonym(Entity& e1, Entity& e2) {
	return e1.isSynonym() && e2.isSynonym();
}

bool QueryOptimizer::isConstantAndConstant(Entity& e1, Entity& e2) {
	return (!e1.isSynonym() && e1.getType() != EntityType::WILD) &&
		(!e2.isSynonym() && e2.getType() != EntityType::WILD);
}

bool QueryOptimizer::isSynonymAndConstant(Entity& e1, Entity& e2) {
	return ((e1.isSynonym() && (!e2.isSynonym() && e2.getType() != EntityType::WILD)) ||
		(e2.isSynonym() && (!e1.isSynonym() && e1.getType() != EntityType::WILD)));
}

bool QueryOptimizer::isNoSynonym(Entity& e1, Entity& e2) {
	return !e1.isSynonym() && !e2.isSynonym();
}

bool QueryOptimizer::isOneSynonym(Entity& e1, Entity& e2) {
	return (!e1.isSynonym() && e2.isSynonym()) || (!e2.isSynonym() && e1.isSynonym());
}

bool QueryOptimizer::isNextTWithCFGSearch(Entity& e1, Entity& e2) {
	return isSynonymAndSynonym(e1, e2) || isSynonymAndConstant(e1, e2) || isConstantAndConstant(e1, e2);
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
