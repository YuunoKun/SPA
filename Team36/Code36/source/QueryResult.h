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
	void getResults(std::vector<Entity>&, ResultTable&);
	void getResult(Entity, std::list<std::string>&);

private:
	std::vector<ResultTable> results;
	std::unordered_set<std::string> header_set;
	bool have_result;

	void addHeader(std::vector<Entity>);
};