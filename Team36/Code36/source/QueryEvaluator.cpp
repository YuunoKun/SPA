#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator() {
}

std::list<std::string> QueryEvaluator::evaluateQuery(Query query) {
	QueryResult result;
	return getResult(query, result);;
}

std::list<std::string> QueryEvaluator::getRawResult(Entity selected) {
	std::list<std::string> result;
	switch (selected.getType()) {
	case STMT: result = stmtsToString(pkb.getStmts()); break;
	case READ: break;
	case PRINT: break;
	case CALL: break;
	case WHILE: break;
	case IF: break;
	case ASSIGN: break;
	case VARIABLE: result = variablesToString(pkb.getVariables());  break;
	case CONSTANT: result = constantsToString(pkb.getConstants());  break;
	case PROCEDURE: result = proceduresToString(pkb.getProcedures());  break;
	}

	return result;
}

std::list<std::string> QueryEvaluator::constantsToString(std::vector<constant> from) {
	std::list<std::string> to;
	for each (constant c in from) {
		to.push_back(std::to_string(c));
	}
	return to;
}

std::list<std::string> QueryEvaluator::proceduresToString(std::vector<procedure_name> from) {
	std::list<std::string> to;
	for each (procedure_name name in from) {
		to.push_back(name.c_str());
	}
	return to;
}

std::list<std::string> QueryEvaluator::stmtsToString(std::vector<Stmt> from) {
	std::list<std::string> to;
	for each (Stmt stmt in from) {
		to.push_back(std::to_string(stmt.getNum()));
	}
	return to;
}

std::list<std::string> QueryEvaluator::variablesToString(std::vector<variable_name> from) {
	std::list<std::string> to;
	for each (variable_name name in from) {
		to.push_back(name.c_str());
	}
	return to;
}

std::list<std::string> QueryEvaluator::getResult(Query& query, QueryResult& result) {
	if (result.isInTable(query.getSelected())) {

	}
	else {
		//if selected type is not in table, return all.
		return getRawResult(query.getSelected());
	}

	//TODO: remove
	return getRawResult(query.getSelected());
}