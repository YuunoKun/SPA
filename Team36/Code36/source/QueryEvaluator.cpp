#include "QueryEvaluator.h"

#include "Utility.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternsEvaluator.h"
#include "Clause.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query& query) {
	QueryResult result;

	evaluateClauses(query, result);

	return getResult(query, result);
}

void QueryEvaluator::evaluateClauses(Query& query, QueryResult& query_result) {
	RelationsEvaluator relation_evaluator;
	PatternsEvaluator pattern_evaluator;
	for (auto& it : query.getClauses()) {
		if (!query_result.haveResult()) {
			break;
		}
		if (it.getType() == ClauseType::RELATION) {
			relation_evaluator.evaluateRelation(query_result, it.getRelation());
		} else {
			pattern_evaluator.evaluatePattern(query_result, it.getPattern());
		}
	}
}


std::list<std::string> QueryEvaluator::getRawResult(Entity selected) {
	if (Utility::isSecondaryAttribute(selected)) {
		return getRawResultWithSecondaryAttribute(selected);
	}
	std::list<std::string> result;
	switch (selected.getType()) {
	case EntityType::PROG_LINE :
	case EntityType::STMT: result = Utility::stmtInfoToStringList(pkb.getStmts()); break;
	case EntityType::READ: result = Utility::stmtInfoToStringList(pkb.getReads());  break;
	case EntityType::PRINT: result = Utility::stmtInfoToStringList(pkb.getPrints()); break;
	case EntityType::CALL: result = Utility::stmtInfoToStringList(pkb.getCalls()); break;
	case EntityType::WHILE: result = Utility::stmtInfoToStringList(pkb.getWhiles()); break;
	case EntityType::IF: result = Utility::stmtInfoToStringList(pkb.getIfs()); break;
	case EntityType::ASSIGN: result = Utility::stmtInfoToStringList(pkb.getAssigns()); break;
	case EntityType::VARIABLE: result = Utility::variablesToStringList(pkb.getVariables());  break;
	case EntityType::CONSTANT: result = Utility::constantsToStringList(pkb.getConstants());  break;
	case EntityType::PROCEDURE: result = Utility::proceduresToStringList(pkb.getProcedures());  break;
	}

	return result;
}

std::list<std::string> QueryEvaluator::getRawResultWithSecondaryAttribute(Entity selected) {
	switch (selected.getType()) {
	case EntityType::READ:
		if (selected.getAttribute() == AttrRef::VAR_NAME) {
			return Utility::variablesToStringList(pkb.getPrintVar());
		}
		break;
	case EntityType::PRINT:
		if (selected.getAttribute() == AttrRef::VAR_NAME) {
			return Utility::variablesToStringList(pkb.getReadVar());
		}
		break;
	case EntityType::CALL:
		if (selected.getAttribute() == AttrRef::PROC_NAME) {
			return Utility::proceduresToStringList(pkb.getCalledS());
		}
		break;
	}

	throw std::domain_error("Selected Entity type does not have Secondary Attribute!");
}


std::list<std::string> QueryEvaluator::getTupleResult(Query& query, QueryResult& query_result) {
	std::vector<Entity> selected_entities = Utility::removeDuplicateEntities(query.getSelected());
	std::list<ResultTable> results;
	if (query_result.isInTables(selected_entities)) {
		results.emplace_back(query_result.getResults(selected_entities));
		selected_entities = Utility::removeEntities(selected_entities, results.front().getHeaders());
	}

	for (auto& selected : selected_entities) {
		results.push_back(ResultTable(selected, getRawResult(selected)));
	}

	ResultTable result = results.front();
	results.pop_front();
	while (!results.empty()) {
		result.joinTable(results.front());
		results.pop_front();
	}
	return result.getEntityResult(query.getSelected());
}


std::list<std::string> QueryEvaluator::getResult(Query& query, QueryResult& result) {
	if (query.getSelected()[0].getType() == EntityType::BOOLEAN) {
		if (result.haveResult()) {
			return { BOOLEAN_TRUE };
		}
		else {
			return { BOOLEAN_FALSE };
		}
	}

	if (!result.haveResult()) {
		return {};
	}

	if (query.getSelected().size() > 1) {
		return getTupleResult(query, result);
	}

	if (!result.isInTables(query.getSelected()[0])) {
		return getRawResult(query.getSelected()[0]);
	}

	return result.getResult(query.getSelected()[0]);
}