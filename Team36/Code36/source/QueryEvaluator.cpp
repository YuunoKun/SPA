#include "QueryEvaluator.h"

#include "Utility.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternsEvaluator.h"
#include "Clause.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::list<std::string>> QueryEvaluator::evaluateQuery(Query& query) {
	QueryResult result;

	evaluateClauses(query, result);

	return { getResult(query, result) };
}

void QueryEvaluator::evaluateClauses(Query& query, QueryResult& queryResult) {
	RelationsEvaluator relationEvaluator;
	PatternsEvaluator patternEvaluator;
	for (auto& it : query.getClauses()) {
		if (!queryResult.haveResult()) {
			break;
		}
		if (it.getType() == ClauseType::RELATION) {
			relationEvaluator.evaluateRelation(queryResult, it.getRelation());
		} else {
			patternEvaluator.evaluatePattern(queryResult, it.getPattern());
		}
	}
}


std::list<std::string> QueryEvaluator::getRawResult(Entity selected) {
	std::list<std::string> result;
	switch (selected.getType()) {
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

std::list<std::string> QueryEvaluator::getResult(Query& query, QueryResult& result) {
	//Return boolean value if select is BOOLEAN
	if (query.getSelected()[0].getType() == EntityType::BOOLEAN) {
		if (result.haveResult()) {
			return { BOOLEAN_TRUE };
		}
		else {
			return { BOOLEAN_FALSE };
		}
	}

	//If some of the relation/pattern return empty list / false
	if (!result.haveResult()) {
		return {};
	}

	if (query.getSelected().size() > 1) {
		//For iteration 2, handling tuples result
	}

	//return non-tuples result
	//If the variable is not found in one of the result table, return all of selected entity type
	if (!result.isInTables(query.getSelected()[0])) {
		return getRawResult(query.getSelected()[0]);
	}

	//If the variable is found in one of the result table
	return result.getResult(query.getSelected()[0]);
}