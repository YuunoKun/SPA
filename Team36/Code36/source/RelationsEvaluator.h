#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "RelationEvaluatorInterface.h"

class RelationsEvaluator {
public:
	void evaluateRelation(QueryResult&, RelRef&);
	void evaluateRelation(QueryResult&, RelRef&, RelationEvaluatorInterface&);
};