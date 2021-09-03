#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class QueryEvaluator {
public:
	QueryEvaluator();

	std::list<std::string> evaluateQuery(Query);

private:
	void evaluateRelations(Query&, QueryResult&);
	void evaluatePatterns(Query&, QueryResult&);

	std::list<std::string> getResult(Query&, QueryResult&);
	std::list<std::string> getRawResult(Entity);

	PKBAdapter pkb;
};
