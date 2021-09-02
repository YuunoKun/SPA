#pragma once

#include "Query.h"
#include "QueryResult.h"

class RelationEvaluator {
public:
	ResultTable evaluateRelation(Query, RelRef);
};