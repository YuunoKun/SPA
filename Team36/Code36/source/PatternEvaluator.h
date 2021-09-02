#pragma once

#include "Query.h"
#include "QueryResult.h"

class PatternEvaluator {
public:
	void evaluatePattern(QueryResult&, Pattern&);
};