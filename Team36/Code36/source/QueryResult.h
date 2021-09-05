#pragma once

#include "Query.h"
#include "ResultTable.h"

class QueryResult {
public:
	QueryResult();

	bool haveResult();
	void setNoResult();
	bool isInTables(Entity);
	void addResult(ResultTable);
	std::list<std::string> getResult(Entity);

private:
	std::vector<ResultTable> results;
	bool have_result;
};