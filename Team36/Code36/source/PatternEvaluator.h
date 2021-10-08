#pragma once

#include "Query.h"
#include "QueryResult.h"
#include "PKBAdapter.h"
#include "Common.h"

class PatternEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);

private:
	PKBAdapter pkb;
};