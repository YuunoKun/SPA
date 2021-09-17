#include "QueryEvaluator.h"

#include "Utility.h"
#include "ResultTable.h"
#include "RelationsEvaluator.h"
#include "PatternEvaluator.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query query) {
	try {
		QueryResult result;

		evaluateRelations(query, result);
		evaluatePatterns(query, result);

		return getResult(query, result);;
	}
	catch (...) {
		//Some error occur, return empty list
		return {};
	}
}

void QueryEvaluator::evaluateRelations(Query& query, QueryResult& queryResult) {
	RelationsEvaluator evaluator;
	for (auto& it : query.getRelations()) {
		if (!queryResult.haveResult()) {
			break;
		}
		evaluator.evaluateRelation(queryResult, it);
	}
}

void QueryEvaluator::evaluatePatterns(Query& query, QueryResult& queryResult) {
	PatternEvaluator evaluator;
	for (auto& it : query.getPatterns()) {
		if (!queryResult.haveResult()) {
			break;
		}
		evaluator.evaluatePattern(queryResult, it);
	}
}

std::list<std::string> QueryEvaluator::getRawResult(Entity selected) {
	std::list<std::string> result;
	switch (selected.getType()) {
	case STMT: result = Utility::stmtInfoToStringList(pkb.getStmts()); break;
	case READ: result = Utility::stmtInfoToStringList(pkb.getReads());  break;
	case PRINT: result = Utility::stmtInfoToStringList(pkb.getPrints()); break;
	case CALL: result = Utility::stmtInfoToStringList(pkb.getCalls()); break;
	case WHILE: result = Utility::stmtInfoToStringList(pkb.getWhiles()); break;
	case IF: result = Utility::stmtInfoToStringList(pkb.getIfs()); break;
	case ASSIGN: result = Utility::stmtInfoToStringList(pkb.getAssigns()); break;
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

	if (query.getSelected().size() > 1) {
		//For iteration 2, handling tuples result
	}

	//Handle non-tuples result
	//If the variable is not found in one of the result table, return all of selected entity type
	if (!result.isInTables(query.getSelected()[0])) {
		return getRawResult(query.getSelected()[0]);
	}

	//If the variable is found in one of the result table
	return result.getResult(query.getSelected()[0]);
}