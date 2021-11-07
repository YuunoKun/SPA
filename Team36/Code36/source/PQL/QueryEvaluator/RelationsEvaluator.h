#pragma once

#include "RelationEvaluator/RelationEvaluatorInterface.h"
#include "QueryResult.h"

class RelationsEvaluator {
public:
	void evaluateRelation(QueryResult&, RelRef&);
private:
	void evaluateRelation(QueryResult&, RelRef&, RelationEvaluatorInterface&);
};