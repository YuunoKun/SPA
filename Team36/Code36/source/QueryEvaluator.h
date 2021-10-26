#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class QueryEvaluator {
public:
	QueryEvaluator();

	std::list<std::string> evaluateQuery(Query&);

private:
	void evaluateClauses(Query&, QueryResult&);

	std::list<std::string> getResult(Query&, QueryResult&);
	std::list<std::string> getRawResult(Entity);
	std::list<std::string> getRawResultWithSecondaryAttribute(Entity);
	std::list<std::string> getTupleResult(Query&, QueryResult&);

	std::string getEntitySecondaryAttribute(std::string, Entity);

	std::list<std::string> converToSecondaryAttribute(std::list<std::string>, Entity);



	std::list<std::string> mergeResultTables(std::list<std::list<std::string>>, std::vector<Entity>);

	PKBAdapter pkb;
};
