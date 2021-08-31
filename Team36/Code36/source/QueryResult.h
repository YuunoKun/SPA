#pragma once

#include "Query.h"

class QueryResult {
public:
	bool haveResult();
	bool isInTable(Entity);
	void join(QueryResult);
};