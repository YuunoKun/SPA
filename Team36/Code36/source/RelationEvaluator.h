#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class RelationEvaluator {
public:
	void evaluateRelation(QueryResult&, RelRef&);

private:
	void evaluateModifiesRelation(QueryResult&, RelRef&);
	void evaluateUsesRelation(QueryResult&, RelRef&);
	void evaluateParentRelation(QueryResult&, RelRef&);
	void evaluateParentTRelation(QueryResult&, RelRef&);
	void evaluateFollowsRelation(QueryResult&, RelRef&);
	void evaluateFollowsTRelation(QueryResult&, RelRef&);


	PKBAdapter pkb;
};