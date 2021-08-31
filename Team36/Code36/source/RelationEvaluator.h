#pragma once

#include "Query.h"
#include "QueryResult.h"

class RelationEvaluator {
public:
	QueryResult evaluateRelation(Query, RelRef);
};