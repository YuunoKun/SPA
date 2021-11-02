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

	void getResult(Query&, QueryResult&, std::list<std::string>&);
	void getRawResult(Entity, std::list<std::string>&);
	void getRawResultWithSecondaryAttribute(Entity, std::list<std::string>&);
	void getTupleResult(Query&, QueryResult&, std::list<std::string>&);

	std::string getEntitySecondaryAttribute(std::string, Entity&);

	void convertToSecondaryAttribute(std::list<std::string>&, Entity&);


	void mergeTable(ResultTable&, std::vector<Entity>&, std::list<std::string>&);

	PKBAdapter pkb;
};
