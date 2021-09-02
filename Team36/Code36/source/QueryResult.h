#pragma once

#include "Query.h"
#include "ResultTable.h"

class QueryResult {
public:
	bool haveResult();
	bool isInTable(Entity);
	void addResult(ResultTable);

private:
	std::vector<ResultTable> results;
	bool have_result;
};