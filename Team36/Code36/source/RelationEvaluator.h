#pragma once

#include "Query.h"
#include "QueryResult.h"

class RelationEvaluator {
public:
	void evaluateRelation(QueryResult&, RelRef&);
};