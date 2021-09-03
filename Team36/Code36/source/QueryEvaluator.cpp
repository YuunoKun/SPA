#include "QueryEvaluator.h"

#include "Utility.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternEvaluator.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query query) {
	QueryResult result;

	evaluateRelations(query, result);
	evaluatePatterns(query, result);

	return getResult(query, result);;
}

void QueryEvaluator::evaluateRelations(Query& query, QueryResult& queryResult) {
	RelationsEvaluator evaluator;
	for (auto& it : query.getRelations()) {
		evaluator.evaluateRelation(queryResult, it);
	}
}

void QueryEvaluator::evaluatePatterns(Query& query, QueryResult& queryResult) {
	PatternEvaluator evaluator;
	for (auto& it : query.getPatterns()) {
		evaluator.evaluatePattern(queryResult, it);
	}
}

std::list<std::string> QueryEvaluator::getRawResult(Entity selected) {
	std::list<std::string> result;
	switch (selected.getType()) {
	case STMT: result = Utility::stmtsToStringList(pkb.getStmts()); break;
	case READ: result = Utility::stmtsToStringList(pkb.getReads());  break;
	case PRINT: result = Utility::stmtsToStringList(pkb.getPrints()); break;
	case CALL: result = Utility::stmtsToStringList(pkb.getCalls()); break;
	case WHILE: result = Utility::stmtsToStringList(pkb.getWhiles()); break;
	case IF: result = Utility::stmtsToStringList(pkb.getIfs()); break;
	case ASSIGN: result = Utility::stmtsToStringList(pkb.getAssigns()); break;
	case VARIABLE: result = Utility::variablesToStringList(pkb.getVariables());  break;
	case CONSTANT: result = Utility::constantsToStringList(pkb.getConstants());  break;
	case PROCEDURE: result = Utility::proceduresToStringList(pkb.getProcedures());  break;
	}

	return result;
}

std::list<std::string> QueryEvaluator::getResult(Query& query, QueryResult& result) {
	//If some of the relation/pattern return empty list / false
	if (!result.haveResult()) {
		return {};
	}

	//If the variable is not found in one of the result table, return all of selected entity type
	if (!result.isInTables(query.getSelected())) {
		return getRawResult(query.getSelected());
	}

	//If the variable is found in one of the result table
	return result.getResult(query.getSelected());
}