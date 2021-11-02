#include "QueryOptimizer.h"
#include "Entity.h"
#include "Utility.h"



QueryOptimizer::QueryOptimizer() {
}

QueryOptimizer::QueryOptimizer(bool optimize_clause_by_common_synonym, bool optimize_clause_by_relation_type) {
	this->optimize_clause_by_common_synonym = optimize_clause_by_common_synonym;
	this->optimize_clause_by_relation_type = optimize_clause_by_relation_type;
}

QueryOptimizer::QueryOptimizer(bool optimize_clause_by_common_synonym, bool optimize_clause_by_relation_type, bool affect_threshold_optimization, int affect_threshold_count) {
	this->optimize_clause_by_common_synonym = optimize_clause_by_common_synonym;
	this->optimize_clause_by_relation_type = optimize_clause_by_relation_type;
	this->affect_threshold_optimization = affect_threshold_optimization;
	this->affect_threshold_count = affect_threshold_count;
}

std::vector<Clause> QueryOptimizer::optimizeClausesOrder(std::vector<Clause>& clauses) {
	std::list<Clause> clauses_list(clauses.begin(), clauses.end());
	if (optimize_clause_by_common_synonym || optimize_clause_by_relation_type) {
		clauses_list = sortClausesByNumOfSynonym(clauses_list);
	}
	if (affect_threshold_optimization) {
		clauses_list = optmizeClausesWithAffectThreshold(clauses_list);
	}
	if (optimize_clause_by_common_synonym) {
		clauses_list = sortClausesByCommonSynonym(clauses_list);
	}
	
	if (optimize_clause_by_relation_type) {
		clauses_list = sortClausesByComputationalType(clauses_list);
	}
	std::vector<Clause> clauses_result(clauses_list.begin(), clauses_list.end());
	return clauses_result;
}


//If the number of affects and affects_t reaches a certain threshold and there is no two synonym clauses
//Add a dummy two synonym clauses to pre-compute and cache the result
std::list<Clause> QueryOptimizer::optmizeClausesWithAffectThreshold(std::list<Clause>& clauses) {

	bool have_affect_table_cache_clauses = false;
	int affect_clauses_count = 0;
	int affect_t_clauses_count = 0;
	std::list<Clause> compute_affect_graph, affect_cfg_search_clauses, affect_t_cfg_search_clauses;

	for (Clause c : clauses) {
		if (c.getType() == ClauseType::PATTERN) {
			continue;
		}

		RelType relationType = c.getRelation().getType();
		if (relationType != RelType::AFFECT && relationType != RelType::AFFECT_T) {
			continue;
		}

		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity) && relationType == RelType::AFFECT_T) {
			//Affect* table will be re-compute, no optimization needed
			return clauses;
		}
		else if (isSynonymAndSynonym(left_entity, right_entity) && relationType == RelType::AFFECT) {
			have_affect_table_cache_clauses = true;
		}
		else if (relationType == RelType::AFFECT_T && isDFSWhenNonTTableNotCached(left_entity, right_entity)) {
			have_affect_table_cache_clauses = true;
			affect_t_clauses_count++;
		}
		else {
			affect_clauses_count++;
		}
	}
	if (affect_t_clauses_count > affect_threshold_count) {
		clauses.push_back({ {AFFECT_T, {ASSIGN, Synonym()}, {ASSIGN, Synonym() } } });
	}
	else if (!have_affect_table_cache_clauses && affect_clauses_count > affect_threshold_count) {
		clauses.push_back({ {AFFECT, {ASSIGN, Synonym()}, {ASSIGN, Synonym() } } });
	}
	return clauses;
}

//Re-order the clauses in the following order
//1. clauses with one or less synonym
//2. clauses with two synonym - sorted by common synonym
std::list<Clause> QueryOptimizer::sortClausesByCommonSynonym(std::list<Clause>& clauses) {
	std::list<Clause> others_synonym_list, two_synonym_list;
	for (Clause c : clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity)) {
			two_synonym_list.emplace_back(c);
		}
		else {
			others_synonym_list.emplace_back(c);
		}
	}

	two_synonym_list = sortTwoSynonymClausesByCommonSynonym(two_synonym_list);

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), others_synonym_list.begin(), others_synonym_list.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_list.begin(), two_synonym_list.end());
	return reordered_clauses;
}


//Re-order the clauses in the following order to delay joining of table 
//1. clauses with no common synonym
//2. clauses with two common synonym only
//3. clauses with one common synonym
std::list<Clause> QueryOptimizer::sortTwoSynonymClausesByCommonSynonym(std::list<Clause>& two_synonym_clauses) {
	std::unordered_map<std::string, int> synonym_count;
	std::vector<std::vector<std::string>> synonym_names;
	for (Clause c : two_synonym_clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);
		if (left_entity.getSynonym() == right_entity.getSynonym()) {
			//Skip as this is the same as having only 1 column, with the need to fliter/combine e.g Affect(n,n)
			continue;
		}
		std::vector<std::string> synonym_name = { left_entity.getSynonym(), right_entity.getSynonym() };
		sort(synonym_name.begin(), synonym_name.end());
		synonym_name.push_back(synonym_name[0] + " " + synonym_name[1]);
		for (std::string name : synonym_name) {

			std::unordered_map<std::string, int>::iterator it = synonym_count.find(name);
			if (it != synonym_count.end()) {
				it->second++;
			} else {
				synonym_count.insert(std::make_pair(name, 1));
			}
		}

		synonym_names.push_back(synonym_name);
	}

	std::list<Clause> no_common_synonym_clauses, two_common_synonym_clauses, one_common_synonym_clauses;

	int count = 0;

	for (Clause c : two_synonym_clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);
		if (left_entity.getSynonym() == right_entity.getSynonym()) {
			no_common_synonym_clauses.emplace_back(c);
			continue;
		}

		int left_synonym_count = synonym_count.find(synonym_names[count][0])->second;
		int right_synonym_count = synonym_count.find(synonym_names[count][1])->second;
		int combine_synonym_count = synonym_count.find(synonym_names[count][2])->second;
		if (left_synonym_count == 1 && right_synonym_count == 1 && combine_synonym_count == 1) {
			no_common_synonym_clauses.emplace_back(c);
		}
		else if (left_synonym_count == right_synonym_count && combine_synonym_count == right_synonym_count) {
			two_common_synonym_clauses.emplace_back(c);
		} else {
			one_common_synonym_clauses.emplace_back(c);
		}
		count++;
	}

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), no_common_synonym_clauses.begin(), no_common_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), two_common_synonym_clauses.begin(), two_common_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), one_common_synonym_clauses.begin(), one_common_synonym_clauses.end());
	return reordered_clauses;

	return two_synonym_clauses;
}


//Re-order the clauses in the following order according to their computational intensity in increasing other.
//1. Other clauses
//2. Next_T clauses that require CFG search
//3. Affect clauses
std::list<Clause> QueryOptimizer::sortClausesByComputationalType(std::list<Clause>& clauses) {
	std::list<Clause> others_clauses, next_t_cfg_search_clauses, affects_clauses;
	for (Clause c : clauses) {
		ClauseType type = c.getType();
		RelType relationType = c.getRelation().getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (type == ClauseType::RELATION && relationType == RelType::NEXT_T
			&& isDFSWhenNonTTableNotCached(left_entity, right_entity)) {
			next_t_cfg_search_clauses.emplace_back(c);
		}
		else if (type == ClauseType::RELATION && (relationType == RelType::AFFECT || relationType == RelType::AFFECT_T)) {
			affects_clauses.emplace_back(c);
		}
		else {
			others_clauses.emplace_back(c);
		}
	}
	next_t_cfg_search_clauses = sortNextTClauses(next_t_cfg_search_clauses);
	affects_clauses = sortAffectsClauses(affects_clauses);

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), others_clauses.begin(), others_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), next_t_cfg_search_clauses.begin(), next_t_cfg_search_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affects_clauses.begin(), affects_clauses.end());
	return reordered_clauses;
}


//Re-order the next_t clauses in the following order
//1. First two common synonym next clauses (to cache the result)
//2. Others clauses
std::list<Clause> QueryOptimizer::sortNextTClauses(std::list<Clause>& next_t_clauses) {
	std::list<Clause> compute_next_graph, other_clauses;

	for (Clause c : next_t_clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity) && compute_next_graph.empty()) {
			compute_next_graph.emplace_back(c);
		}
		else {
			other_clauses.emplace_back(c);
		}
	}
	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), compute_next_graph.begin(), compute_next_graph.end());
	reordered_clauses.insert(reordered_clauses.end(), other_clauses.begin(), other_clauses.end());
	return reordered_clauses;
}


//Re-order the afffects clauses in the following order
//1. first two common synonym affect* or affect clauses (to cache the result)
//2. Affects/Affect_T clauses that require graph search with next table
//3. Affects_T clauses that require graph search on pre-compute affect table
std::list<Clause> QueryOptimizer::sortAffectsClauses(std::list<Clause>& affect_clauses) {

	std::list<Clause> compute_affect_graph, affect_cfg_search_clauses, affect_t_cfg_search_clauses;

	for (Clause c : affect_clauses) {
		RelType relationType = c.getRelation().getType();
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity)) {
			if (compute_affect_graph.empty()) {
				compute_affect_graph.emplace_back(c);
			}
			else if(compute_affect_graph.front().getRelation().getType() == RelType::AFFECT && relationType == RelType::AFFECT_T){
				compute_affect_graph.emplace_back(c);
				affect_cfg_search_clauses.emplace_back(compute_affect_graph.front());
				compute_affect_graph.pop_front();
			}
			else if (relationType == RelType::AFFECT_T) {
				affect_t_cfg_search_clauses.emplace_back(c);
			}
			else {
				affect_cfg_search_clauses.emplace_back(c);
			}

		}else if (relationType == RelType::AFFECT_T && isDFSWhenNonTTableNotCached(left_entity, right_entity)) {
			affect_t_cfg_search_clauses.emplace_back(c);
		}
		else{
			affect_cfg_search_clauses.emplace_back(c);
		}
	}

	//If there need to pre-compute affect table for some affect* clauses, compute the affect* clauses first to cache the result
	if (compute_affect_graph.size() == 0 && affect_t_cfg_search_clauses.size() > 0) {
		compute_affect_graph.push_back(affect_t_cfg_search_clauses.front());
		affect_t_cfg_search_clauses.pop_front();
	}


	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), compute_affect_graph.begin(), compute_affect_graph.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_cfg_search_clauses.begin(), affect_cfg_search_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), affect_t_cfg_search_clauses.begin(), affect_t_cfg_search_clauses.end());
	return reordered_clauses;
}


//Re-order the affects t clauses in the following order
//2. Affect_T clauses that require graph search and terminate whenever a single result found
//3. Affect_T clauses that require graph search and terminate when all relevant result is found
//2. Two synonym clauses
std::list<Clause> QueryOptimizer::sortClausesByNumOfSynonym(std::list<Clause>& clauses) {
	std::list<Clause> no_synonym_clauses,
		one_synonym_clauses, two_synonym_clauses;

	for (Clause c : clauses) {
		Entity left_entity = getLeftEntity(c);
		Entity right_entity = getRightEntity(c);

		if (isSynonymAndSynonym(left_entity, right_entity)) {
			two_synonym_clauses.emplace_back(c);
		}
		else if (isNoSynonym(left_entity, right_entity)) {
			no_synonym_clauses.emplace_back(c);
		}
		else if (isOneSynonym(left_entity, right_entity)) {
			one_synonym_clauses.emplace_back(c);
		}
		else {
			throw std::domain_error("optimizeAffectsTClausesOrder() :: Some next clauses has not been handle!");
		}

	}

	std::list<Clause> reordered_clauses;
	reordered_clauses.insert(reordered_clauses.end(), no_synonym_clauses.begin(), no_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), one_synonym_clauses.begin(), one_synonym_clauses.end());
	reordered_clauses.insert(reordered_clauses.end(), two_synonym_clauses.begin(), two_synonym_clauses.end());
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
	if (e.isSynonym() || e.getType() == EntityType::WILD || e.getType() == EntityType::BOOLEAN) {
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

//The synonym combination require to perform DFS search on Non-T table when not cached
// e.g Next_T perform DFS on Next Table or Affect_T perform DFS on Affect table
bool QueryOptimizer::isDFSWhenNonTTableNotCached(Entity& e1, Entity& e2) {
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
