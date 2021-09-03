#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"

class RelationsEvaluator {
public:
	void evaluateRelation(QueryResult&, RelRef&);

private:
	void evaluateFollowsRelation(QueryResult&, RelRef&);
	void evaluateFollowsTRelation(QueryResult&, RelRef&);
	void evaluateParentRelation(QueryResult&, RelRef&);
	void evaluateParentTRelation(QueryResult&, RelRef&);
	void evaluateModifiesRelation(QueryResult&, RelRef&);
	void evaluateUsesRelation(QueryResult&, RelRef&);


	PKBAdapter pkb;
};