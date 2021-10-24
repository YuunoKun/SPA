#pragma once

#include "Query.h"
#include "ResultTable.h"

class QueryResult {
public:
	QueryResult();

	bool haveResult();
	void setNoResult();
	bool isInTables(std::vector<Entity>);
	bool isInTables(Entity);
	void addResult(ResultTable);
	ResultTable getResults(std::vector<Entity>);
	std::list<std::string> getResult(Entity);

private:
	std::vector<ResultTable> results;
	std::unordered_set<std::string> header_set;
	bool have_result;

	void addHeader(std::vector<Entity>);
};