#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class QueryEvaluator {
public:
	QueryEvaluator();

	std::list<std::string> evaluateQuery(Query);

private:

	std::list<std::string> getResult(Query&, QueryResult&);
	std::list<std::string> getRawResult(Entity);


	std::list<std::string> constantsToString(std::vector<constant>);
	std::list<std::string> proceduresToString(std::vector<procedure_name>);
	std::list<std::string> stmtsToString(std::vector<Stmt>);
	std::list<std::string> variablesToString(std::vector<variable_name>);

	PKBAdapter pkb;
};
